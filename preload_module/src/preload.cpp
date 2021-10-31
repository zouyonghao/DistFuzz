#include <arpa/inet.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>

#include <dst_random.h>
#include <utils/share_mem_util.h>

#include <cstdio>
#include <cstdlib>
#include <functional>
#include <future>
#include <vector>

/**
 * TODO: fprintf, fgets, fread, and fwrite?
 * read(), write()
 * readv(), writev()
 * pread(), pwrite()
 * recv(), send()
 * recvfrom(), sendto()
 * recvmsg(), sendmsg() (also used for sending FDs to other processes over a Unix domain socket)
 * recvmmsg(), sendmmsg()
 */

ssize_t (*k_read)(int fd, void *bf, size_t count);
ssize_t (*k_write)(int fd, const void *buf, size_t len);
ssize_t (*k_readv)(int fd, const struct iovec *iov, int iovcnt);
ssize_t (*k_writev)(int fd, const struct iovec *iov, int iovcnt);
ssize_t (*k_pread)(int fd, void *buf, size_t count, off_t offset);
ssize_t (*k_pwrite)(int fd, const void *buf, size_t count, off_t offset);
ssize_t (*k_preadv)(int fd, const struct iovec *iov, int iovcnt, off_t offset);
ssize_t (*k_pwritev)(int fd, const struct iovec *iov, int iovcnt, off_t offset);
ssize_t (*k_preadv2)(int fd, const struct iovec *iov, int iovcnt, off_t offset, int flags);
ssize_t (*k_pwritev2)(int fd, const struct iovec *iov, int iovcnt, off_t offset, int flags);
ssize_t (*k_send)(int fd, const void *buf, size_t len, int flags);
ssize_t (*k_sendto)(int fd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr,
                    socklen_t addrlen);
ssize_t (*k_sendmsg)(int fd, const struct msghdr *msg, int flags);
ssize_t (*k_recv)(int sockfd, void *buf, size_t len, int flags);
ssize_t (*k_recvfrom)(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen);
ssize_t (*k_recvmsg)(int sockfd, struct msghdr *msg, int flags);

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
    INIT_FUNC(read, handle, ssize_t(*)(int fd, void *bf, size_t count));
    INIT_FUNC(write, handle, ssize_t(*)(int fd, const void *buf, size_t len));
    INIT_FUNC(readv, handle, ssize_t(*)(int fd, const struct iovec *iov, int iovcnt));
    INIT_FUNC(writev, handle, ssize_t(*)(int fd, const struct iovec *iov, int iovcnt));
    INIT_FUNC(pread, handle, ssize_t(*)(int fd, void *buf, size_t count, off_t offset));
    INIT_FUNC(pwrite, handle, ssize_t(*)(int fd, const void *buf, size_t count, off_t offset));
    INIT_FUNC(preadv, handle, ssize_t(*)(int fd, const struct iovec *iov, int iovcnt, off_t offset));
    INIT_FUNC(pwritev, handle, ssize_t(*)(int fd, const struct iovec *iov, int iovcnt, off_t offset));
    INIT_FUNC(preadv2, handle, ssize_t(*)(int fd, const struct iovec *iov, int iovcnt, off_t offset, int flags));
    INIT_FUNC(pwritev2, handle, ssize_t(*)(int fd, const struct iovec *iov, int iovcnt, off_t offset, int flags));
    INIT_FUNC(send, handle, ssize_t(*)(int fd, const void *buf, size_t len, int flags));
    INIT_FUNC(sendto, handle,
              ssize_t(*)(int fd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr,
                         socklen_t addrlen));
    INIT_FUNC(sendmsg, handle, ssize_t(*)(int fd, const struct msghdr *msg, int flags));
    INIT_FUNC(recv, handle, ssize_t(*)(int sockfd, void *buf, size_t len, int flags));
    INIT_FUNC(recvfrom, handle,
              ssize_t(*)(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen));
    INIT_FUNC(recvmsg, handle, ssize_t(*)(int sockfd, struct msghdr *msg, int flags));

    fprintf(stderr, "symbol_init success!\n");
}

int get_file_path(int fd, struct stat *sb, char **buf)
{
    /* Add one to the link size, so that we can determine whether
              the buffer returned by readlink() was truncated. */
    int bufsiz = sb->st_size + 1;

    /* Some magic symlinks under (for example) /proc and /sys
       report 'st_size' as zero. In that case, take PATH_MAX as
       a "good enough" estimate. */
    if (sb->st_size == 0)
        bufsiz = PATH_MAX;

    *buf = (char *)malloc(bufsiz);
    if (*buf == NULL)
    {
        perror("malloc");
        return -1;
    }
    std::string path = "/proc/self/fd/" + std::to_string(fd);
    ssize_t nbytes = readlink(path.c_str(), *buf, bufsiz);
    if (nbytes == -1)
    {
        perror("readlink");
        return -1;
    }

    /* Print only 'nbytes' of 'buf', as it doesn't contain a terminating
       null byte ('\0'). */
    fprintf(stderr, "'%s' points to '%.*s'\n", path.c_str(), (int)nbytes, *buf);

    /* If the return value was equal to the buffer size, then the
       the link target was larger than expected (perhaps because the
       target was changed between the call to lstat() and the call to
       readlink()). Warn the user that the returned target may have
       been truncated. */

    if (nbytes == bufsiz)
        printf("(Returned buffer may have been truncated)\n");

    return 0;
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
timer_killer _timer_killer;
std::vector<std::future<void>> futures;
bool printed_fuzzing_stopped = false;

ssize_t handle_random_event(const char *func_name, int fd, size_t length, const std::function<ssize_t()> &kernel_func)
{
    if (fd < 4)
    {
        return kernel_func();
    }
    /** If fuzzing is disabled, we call the original function. */
    if (!get_is_fuzzing())
    {
        _timer_killer.kill();
        futures.clear();
        if (!printed_fuzzing_stopped)
        {
            fprintf(stderr, "PRELOAD: fuzzing is stopped, will use kernel_func directly.\n");
            printed_fuzzing_stopped = true;
        }
        return kernel_func();
    }

    /** decease the probability of action */
    if (__dst_get_random_uint8_t() < 150)
    {
        return kernel_func();
    }

    struct stat sb;
    if (fstat(fd, &sb) == -1)
    {
        perror("fstat:");
        return kernel_func();
    }
    switch (sb.st_mode & S_IFMT)
    {
    /** regular file */
    case S_IFREG:
    {
        char *file_path;
        if (get_file_path(fd, &sb, &file_path) == -1)
        {
            perror("get_file_path:");
            return kernel_func();
        }

        if (std::string(file_path).find(".jar") != std::string::npos)
        {
            return kernel_func();
        }

        break;
    }
    /** socket */
    case S_IFSOCK:
    {
        sockaddr_in addr{};
        socklen_t len = sizeof(addr);

        getpeername(fd, (struct sockaddr *)&addr, &len);

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
        break;
    }
    default:
        return kernel_func();
    }

    ssize_t real_write;
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
        if (!_timer_killer.wait_for(std::chrono::microseconds(random)))
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
                                     [length, &kernel_func, &real_write]
                                     {
                                         /** sleep from 0s ~ 1.3s */
                                         uint16_t random = __dst_get_random_uint16_t() * 20;
                                         if (!_timer_killer.wait_for(std::chrono::microseconds(random)))
                                         {
                                             fprintf(stderr, "timer got killed!\n");
                                         }
                                         std::lock_guard<std::mutex> lk(lock_for_write);
                                         // __dst_event_trigger(
                                         //     ("sleep for " + std::to_string(random) + "n").c_str());

                                         real_write = kernel_func();
                                     }));

        return length;
    }
    case DUP:
    {
        real_write = kernel_func();
        break;
    }
    default:
        break;
    }

    std::lock_guard<std::mutex> lk(lock_for_write);
    real_write = kernel_func();
    return real_write;
}

ssize_t get_length_from_iovec(const struct iovec *iov, int iovcnt)
{
    ssize_t length = 0;
    for (int i = 0; i < iovcnt; i++)
    {
        length += iov[i].iov_len;
    }
    return length;
}

ssize_t get_length_from_msghdr(const struct msghdr *msg)
{
    ssize_t length = 0;
    for (int i = 0; i < msg->msg_iovlen; i++)
    {
        length += msg->msg_iov[i].iov_len;
    }
    return length;
}

extern "C" ssize_t read(int fd, void *bf, size_t count)
{
    return handle_random_event("read", fd, count, [&]() { return k_read(fd, bf, count); });
}

extern "C" ssize_t write(int fd, const void *buf, size_t len)
{
    return handle_random_event("write", fd, len, [&]() { return k_write(fd, buf, len); });
}

extern "C" ssize_t readv(int fd, const struct iovec *iov, int iovcnt)
{
    return handle_random_event("readv", fd, get_length_from_iovec(iov, iovcnt),
                               [&]() { return k_readv(fd, iov, iovcnt); });
}

extern "C" ssize_t writev(int fd, const struct iovec *iov, int iovcnt)
{
    return handle_random_event("writev", fd, get_length_from_iovec(iov, iovcnt),
                               [&]() { return k_writev(fd, iov, iovcnt); });
}

extern "C" ssize_t pread(int fd, void *buf, size_t count, off_t offset)
{
    return handle_random_event("pread", fd, count, [&]() { return k_pread(fd, buf, count, offset); });
}

extern "C" ssize_t pwrite(int fd, const void *buf, size_t count, off_t offset)
{
    return handle_random_event("pwrite", fd, count, [&]() { return k_pwrite(fd, buf, count, offset); });
}

extern "C" ssize_t preadv(int fd, const struct iovec *iov, int iovcnt, off_t offset)
{
    return handle_random_event("preadv", fd, get_length_from_iovec(iov, iovcnt),
                               [&]() { return k_preadv(fd, iov, iovcnt, offset); });
}

extern "C" ssize_t pwritev(int fd, const struct iovec *iov, int iovcnt, off_t offset)
{
    return handle_random_event("pwritev", fd, get_length_from_iovec(iov, iovcnt),
                               [&] { return k_pwritev(fd, iov, iovcnt, offset); });
}

extern "C" ssize_t preadv2(int fd, const struct iovec *iov, int iovcnt, off_t offset, int flags)
{
    return handle_random_event("preadv2", fd, get_length_from_iovec(iov, iovcnt),
                               [&]() { return k_preadv2(fd, iov, iovcnt, offset, flags); });
}

extern "C" ssize_t pwritev2(int fd, const struct iovec *iov, int iovcnt, off_t offset, int flags)
{
    return handle_random_event("pwritev2", fd, get_length_from_iovec(iov, iovcnt),
                               [&]() { return k_pwritev2(fd, iov, iovcnt, offset, flags); });
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
    return handle_random_event("sendmsg", fd, get_length_from_msghdr(msg), [&]() { return k_sendmsg(fd, msg, flags); });
}

extern "C" ssize_t recv(int sockfd, void *buf, size_t len, int flags)
{
    return handle_random_event("recv", sockfd, len, [&]() { return k_recv(sockfd, buf, len, flags); });
}

extern "C" ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen)
{
    return handle_random_event("recvfrom", sockfd, len,
                               [&]() { return k_recvfrom(sockfd, buf, len, flags, src_addr, addrlen); });
}

extern "C" ssize_t recvmsg(int sockfd, struct msghdr *msg, int flags)
{
    return handle_random_event("recvmsg", sockfd, get_length_from_msghdr(msg),
                               [&]() { return k_recvmsg(sockfd, msg, flags); });
}