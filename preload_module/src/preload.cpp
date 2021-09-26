#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <dlfcn.h>
#include <dst_random.h>

#include <functional>
#include <future>

#include <arpa/inet.h>
#include <netinet/in.h>

ssize_t (*k_send)(int sockfd, const void *buf, size_t len, int flags);
ssize_t (*k_sendto)(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr,
                    socklen_t addrlen);
ssize_t (*k_sendmsg)(int sockfd, const struct msghdr *msg, int flags);
ssize_t (*k_write)(int fd, const void *buf, size_t len);
ssize_t (*k_writev)(int fd, const struct iovec *iov, int iovcnt);

#define INIT_FUNC(func, handle, type)                                                                                  \
    do                                                                                                                 \
    {                                                                                                                  \
        k_##func = (type)dlsym(handle, #func);                                                                         \
        if ((error = dlerror()) != NULL)                                                                               \
        {                                                                                                              \
            perror(#func "is not init");                                                                               \
        }                                                                                                              \
    } while (0)

static void __attribute__((constructor)) symbol_init(void)
{
    void *handle;
    char *error;

    handle = dlopen("libc.so.6", RTLD_NOW);
    error = dlerror();
    if (!handle)
    {
        fprintf(stderr, "%s\n", error);
        exit(EXIT_FAILURE);
    }
    INIT_FUNC(send, handle, ssize_t(*)(int sockfd, const void *buf, size_t len, int flags));
    INIT_FUNC(sendto, handle,
              ssize_t(*)(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr,
                         socklen_t addrlen));
    INIT_FUNC(sendmsg, handle, ssize_t(*)(int sockfd, const struct msghdr *msg, int flags));
    INIT_FUNC(write, handle, ssize_t(*)(int fd, const void *buf, size_t count));
    INIT_FUNC(writev, handle, ssize_t(*)(int fd, const struct iovec *iov, int iovcnt));
    fprintf(stderr, "symbol_init success!\n");
}

enum SUPPORTED_ACTION
{
    NOOP,
    // REORDER,
    LOST,
    DELAY,
    ASYNC_DELAY, // TODO: think about this argument: async delay is reorder?
    DUP,
    ACTION_COUNT
};

ssize_t handle_random_event(const char *func_name, int fd, ssize_t length, std::function<ssize_t()> kernel_func)
{

    // fprintf(stderr, "%s called.\n", func_name);
    unsigned int val = 0;
    unsigned int val_len = sizeof(val);
    if (0 != getsockopt(fd, SOL_SOCKET, SO_TYPE, &val, &val_len))
    {
        // this is not a socket file descriptor
        return kernel_func();
    }

    sockaddr_in addr;
    socklen_t len = sizeof(addr);
    // getsockname(fd, (struct sockaddr *)&addr, &len);

    // fprintf(stderr, "Local IP address is: %s\n", inet_ntoa(addr.sin_addr));
    // fprintf(stderr, "socket port is %d\n", addr.sin_port);

    getpeername(fd, (struct sockaddr *)&addr, &len);
    // fprintf(stderr, "peer IP address is: %s\n", inet_ntoa(addr.sin_addr));
    // fprintf(stderr, "socket peer port is %d\n", addr.sin_port);

    // server must use ip 127.0.1.1
    in_addr addr_cmp;
    inet_aton("127.0.1.1", &addr_cmp);

    // If the IP is not from 127.0.1.1, then treat it as a client.
    // FIXME: This still can not distinguish the client perfectly
    if (addr_cmp.s_addr != addr.sin_addr.s_addr)
    {
        // be careful when process this...
        // fprintf(stderr, "peer IP address not equals to 127.0.1.1, skip...\n");
        return kernel_func();
    }

    // decease the probability of action
    if (__dst_get_random_uint8_t() < 200)
    {
        return kernel_func();
    }

    static std::mutex lock_for_write;
    // this is a socket file descriptor
    uint8_t select_random = __dst_get_random_uint8_t() % ACTION_COUNT;
    // fprintf(stderr, "action is %d\n", select_random);
    switch (select_random)
    {
    case NOOP:
    {
        break;
    }
    case DELAY:
    {
        uint32_t random = __dst_get_random_uint32();
        usleep(random);
        // __dst_event_trigger(
        //     ("sleep for " + std::to_string(random) + "n").c_str());
        break;
    }
    case LOST:
    {
        return __dst_get_random_uint8_t() % 2 == 0 ? length : 0;
    }
    case ASYNC_DELAY:
    {
        std::async(std::launch::async,
                   [length, kernel_func]
                   {
                       uint32_t random = __dst_get_random_uint32();
                       usleep(random);
                       std::lock_guard<std::mutex> lk(lock_for_write);
                       // __dst_event_trigger(
                       //     ("sleep for " + std::to_string(random) + "n").c_str());

                       ssize_t real_write = kernel_func();
                       if (real_write < length)
                       {
                           fprintf(stderr, "MAY WRITE MULTI TIMES, DANGEROUS!\n");
                       }
                   });

        return length;
    }
    case DUP:
    {
        ssize_t real_write = kernel_func();
        if (real_write < length)
        {
            fprintf(stderr, "MAY WRITE MULTI TIMES, DANGEROUS!\n");
        }
        break;
    }
    default:
        break;
    }

    std::lock_guard<std::mutex> lk(lock_for_write);
    ssize_t real_write = kernel_func();
    if (real_write < length)
    {
        fprintf(stderr, "MAY WRITE MULTI TIMES, DANGEROUS!\n");
    }
    return real_write;
}

/* for send */
extern "C" ssize_t send(int fd, const void *buf, size_t len, int flags)
{
    return handle_random_event("send", fd, len, std::bind(k_send, fd, buf, len, flags));
}

extern "C" ssize_t sendto(int fd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr,
                          socklen_t addrlen)
{
    return handle_random_event("sendto", fd, len, std::bind(k_sendto, fd, buf, len, flags, dest_addr, addrlen));
}

extern "C" ssize_t sendmsg(int fd, const struct msghdr *msg, int flags)
{
    ssize_t length = 0;
    for (int i = 0; i < msg->msg_iovlen; i++)
    {
        length += msg->msg_iov[i].iov_len;
    }
    return handle_random_event("sendmsg", fd, length, std::bind(k_sendmsg, fd, msg, flags));
}

extern "C" ssize_t write(int fd, const void *buf, size_t len)
{
    return handle_random_event("write", fd, len, std::bind(k_write, fd, buf, len));
}

extern "C" ssize_t writev(int fd, const struct iovec *iov, int iovcnt)
{
    ssize_t length = 0;
    for (int i = 0; i < iovcnt; i++)
    {
        length += iov[i].iov_len;
    }
    return handle_random_event("writev", fd, length, std::bind(k_writev, fd, iov, iovcnt));
}