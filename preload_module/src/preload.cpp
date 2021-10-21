#include <cstdio>
#include <sys/socket.h>
#include <unistd.h>

#include <dlfcn.h>
#include <dst_random.h>

#include <functional>
#include <future>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <utils/share_mem_util.h>
#include <vector>

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
    ASYNC_DELAY, /** TODO: think about this question: Is async delay same to reorder? */
    DUP,
    ACTION_COUNT
};

struct timer_killer
{
    /** returns false if interrupted. */
    template <class R, class P> bool wait_for(std::chrono::duration<R, P> const &time)
    {
        auto wait_time = std::chrono::microseconds(0);
        auto ms_100 = std::chrono::microseconds(100);
        bool return_val = false;
        // fprintf(stderr, "wait for %ld seconds\n", (duration_cast<std::chrono::seconds>(time)).count());
        while (!terminate && wait_time < time)
        {
            if (!get_is_fuzzing())
            {
                terminate = true;
            }
            std::unique_lock<std::mutex> lock(m);
            return_val = cv.wait_for(lock, ms_100, [&] { return terminate; });
            wait_time += ms_100;
        }
        return !return_val;
    }
    void kill()
    {
        std::unique_lock<std::mutex> lock(m);
        terminate = true;
        cv.notify_all();
    }

private:
    std::condition_variable cv;
    std::mutex m;
    bool terminate = false;
};

/** global variables */
timer_killer timer_killer;
std::vector<std::future<void>> futures;
bool printed_fuzzing_stopped = false;

ssize_t handle_random_event(const char *func_name, int fd, size_t length, const std::function<ssize_t()> &kernel_func)
{
    // fprintf(stderr, "%s called.\n", func_name);
    unsigned int val = 0;
    unsigned int val_len = sizeof(val);

    /** If fuzzing is disabled, we call the original function. */
    if (!get_is_fuzzing())
    {
        timer_killer.kill();
        futures.clear();
        if (!printed_fuzzing_stopped)
        {
            fprintf(stderr, "PRELOAD: fuzzing is stopped, will use kernel_func directly.\n");
            printed_fuzzing_stopped = true;
        }
        return kernel_func();
    }

    /** If the file descriptor is not a valid network socket, we call the original function.*/
    if (0 != getsockopt(fd, SOL_SOCKET, SO_TYPE, &val, &val_len))
    {
        return kernel_func();
    }

    sockaddr_in addr{};
    socklen_t len = sizeof(addr);
    // getsockname(fd, (struct sockaddr *)&addr, &len);

    // fprintf(stderr, "Local IP address is: %s\n", inet_ntoa(addr.sin_addr));
    // fprintf(stderr, "socket port is %d\n", addr.sin_port);

    getpeername(fd, (struct sockaddr *)&addr, &len);
    // fprintf(stderr, "peer IP address is: %s\n", inet_ntoa(addr.sin_addr));
    // fprintf(stderr, "socket peer port is %d\n", addr.sin_port);

    // server must use ip 127.0.1.1
    in_addr addr_cmp{};
    inet_aton("127.0.1.1", &addr_cmp);

    /**
     * If the IP is not from 127.0.1.1, then treat it as a client.
     * FIXME: This still can not distinguish the client perfectly
     */
    if (addr_cmp.s_addr != addr.sin_addr.s_addr)
    {
        // be careful when process this...
        // fprintf(stderr, "peer IP address not equals to 127.0.1.1, skip...\n");
        return kernel_func();
    }

    /** decease the probability of action */
    if (__dst_get_random_uint8_t() < 200)
    {
        return kernel_func();
    }

    static std::mutex lock_for_write;
    static std::mutex lock_for_vector;
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
        /** sleep from 0s ~ 1.3s */
        uint16_t random = __dst_get_random_uint16_t() * 20;
        if (!timer_killer.wait_for(std::chrono::microseconds(random)))
        {
            fprintf(stderr, "timer got killed!\n");
        }
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
        std::lock_guard<std::mutex> lk(lock_for_vector);
        /** The result of std::async should be stored, otherwise it is not async. */
        futures.push_back(std::async(std::launch::async,
                                     [length, kernel_func]
                                     {
                                         /** sleep from 0s ~ 1.3s */
                                         uint16_t random = __dst_get_random_uint16_t() * 20;
                                         if (!timer_killer.wait_for(std::chrono::microseconds(random)))
                                         {
                                             fprintf(stderr, "timer got killed!\n");
                                         }
                                         std::lock_guard<std::mutex> lk(lock_for_write);
                                         // __dst_event_trigger(
                                         //     ("sleep for " + std::to_string(random) + "n").c_str());

                                         ssize_t real_write = kernel_func();
                                         if (real_write < length)
                                         {
                                             fprintf(stderr, "MAY WRITE MULTI TIMES, DANGEROUS!\n");
                                         }
                                     }));

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
    return handle_random_event("send", fd, len, [&]() { return k_send(fd, buf, len, flags); });
}

extern "C" ssize_t sendto(int fd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr,
                          socklen_t addrlen)
{
    return handle_random_event("sendto", fd, len, [&]() { return k_sendto(fd, buf, len, flags, dest_addr, addrlen); });
}

extern "C" ssize_t sendmsg(int fd, const struct msghdr *msg, int flags)
{
    ssize_t length = 0;
    for (int i = 0; i < msg->msg_iovlen; i++)
    {
        length += msg->msg_iov[i].iov_len;
    }
    return handle_random_event("sendmsg", fd, length, [&]() { return k_sendmsg(fd, msg, flags); });
}

extern "C" ssize_t write(int fd, const void *buf, size_t len)
{
    return handle_random_event("write", fd, len, [&]() { return k_write(fd, buf, len); });
}

extern "C" ssize_t writev(int fd, const struct iovec *iov, int iovcnt)
{
    ssize_t length = 0;
    for (int i = 0; i < iovcnt; i++)
    {
        length += iov[i].iov_len;
    }
    // return handle_random_event("writev", fd, length, std::bind(k_writev, fd, iov, iovcnt));
    return handle_random_event("writev", fd, length, [&]() { return k_writev(fd, iov, iovcnt); });
}