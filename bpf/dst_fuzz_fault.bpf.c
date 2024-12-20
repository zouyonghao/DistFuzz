#include "vmlinux.h"
#include <bpf/bpf_core_read.h>
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>
// #include <linux/errno.h>
#include "utils.h"
#include <linux/errno.h>
#include <linux/limits.h>

int pid = 0;

/**
 * this is used for determining whether the file
 * descriptor should be recorded created by open
 * and openat.
 */
unsigned char should_record[SHOULD_RECORD_SIZE];

/* this records filtered files and sockets */
unsigned long record_fds[FD_SIZE];

/* fuzz_bytes determines whether a syscall injected */
unsigned char fuzz_bytes[FUZZ_BYTES_SIZE];

unsigned volatile int fuzz_bit_index = 0;

uint64_t get_length_from_iovec(const struct iovec *iov, int iovcnt);

/* fuzz_coverage_map records coverage */
unsigned char fuzz_coverage_map[FUZZ_COVERAGE_MAP_SIZE];
unsigned int node_id = 0;
unsigned long event_index = 0;
static inline void record_coverage(bool is_send, int length, unsigned int fd, bool fault_injected)
{
    // unsigned long hash_value =
    //     (((event_index + length + fd + node_id * 100) << is_send) << fault_injected) % FUZZ_COVERAGE_MAP_SIZE;
    unsigned long hash_value =
        ((event_index + length + fd * 97 + node_id * 100 + is_send * 10) << fault_injected) % FUZZ_COVERAGE_MAP_SIZE;
    unsigned long tmp = hash_value - 1;
    if (tmp >= 0 && tmp < FUZZ_COVERAGE_MAP_SIZE && fuzz_coverage_map[tmp] > 0)
    {
        hash_value = (hash_value - 1) % FUZZ_COVERAGE_MAP_SIZE;
    }

    tmp = hash_value + 1;
    if (tmp >= 0 && tmp < FUZZ_COVERAGE_MAP_SIZE && fuzz_coverage_map[tmp] > 0)
    {
        hash_value = (hash_value + 1) % FUZZ_COVERAGE_MAP_SIZE;
    }

    if (hash_value >= 0 && hash_value < FUZZ_COVERAGE_MAP_SIZE)
    {
        fuzz_coverage_map[hash_value]++;
    }
    event_index++;
}

unsigned long long dev;
unsigned long long ino;
static inline int is_current_pid_or_tgid(int pid)
{
    struct bpf_pidns_info ns;
    if (bpf_get_ns_current_pid_tgid(dev, ino, &ns, sizeof(ns)) == 0)
    {
        if (ns.pid == pid || ns.tgid == pid)
        {
            return true;
        }
    }

    u32 current_pid = bpf_get_current_pid_tgid() >> 32;
    u32 current_tgid = bpf_get_current_pid_tgid();

    if (current_pid == pid || current_tgid == pid)
    {
        return true;
    }
    return false;
}

static inline int filter_by_filename(void *filename)
{
    char fname[256];
    bpf_probe_read(&fname, sizeof(fname), filename);

    if (str_contains(fname, ".o", sizeof(fname), 2))
    {
        return 0;
    }
    if (str_contains(fname, ".so", sizeof(fname), 3))
    {
        return 0;
    }
    if (str_contains(fname, ".cfg", sizeof(fname), 4))
    {
        return 0;
    }
    if (str_contains(fname, "lib", sizeof(fname), 3))
    {
        return 0;
    }
    if (str_contains(fname, "/sys", sizeof(fname), 4))
    {
        return 0;
    }
    if (str_contains(fname, "/proc", sizeof(fname), 5))
    {
        return 0;
    }
    if (str_contains(fname, "/dev", sizeof(fname), 4))
    {
        return 0;
    }
    if (str_contains(fname, ".pro", sizeof(fname), 4))
    {
        return 0;
    }
    if (str_contains(fname, ".jar", sizeof(fname), 4))
    {
        return 0;
    }
    if (str_contains(fname, "/var", sizeof(fname), 4))
    {
        return 0;
    }
    if (str_contains(fname, "tmp", sizeof(fname), 3))
    {
        return 0;
    }

    /** NOTE: let's make verifier happy with the volatile and a = index */
    unsigned int current_pid = bpf_get_current_pid_tgid() >> 32;
    volatile int index = current_pid % SHOULD_RECORD_SIZE;
    int a = index;
    if (a >= 0 && a < SHOULD_RECORD_SIZE && fname[0] != 0)
    {
        bpf_printk("record file %s", fname);
        should_record[a] = 1;
    }

    return 0;
}

static inline void record_fd(long ret)
{
    /** NOTE: let's make verifier happy with the volatile and a = index */
    unsigned int current_pid = bpf_get_current_pid_tgid() >> 32;
    volatile int index = current_pid % SHOULD_RECORD_SIZE;
    int a = index;

    if (a >= 0 && a < SHOULD_RECORD_SIZE)
    {
        if (should_record[a])
        {
            should_record[a] = 0;
            volatile int fd_index = (unsigned long)ret % FD_SIZE;
            int b = fd_index;
            if (ret > 0 && b >= 0 && b < FD_SIZE)
            {
                record_fds[b] = 1;
                bpf_printk("record fd %d", ret);
            }
        }
    }
}

SEC("kprobe/__x64_sys_open")
int BPF_KPROBE(__x64_sys_open, const char *filename)
{
    if (!is_current_pid_or_tgid(pid))
    {
        return 0;
    }

    struct pt_regs *new_ctx = PT_REGS_SYSCALL_REGS(ctx);
    filter_by_filename((void *)PT_REGS_PARM1_CORE_SYSCALL(new_ctx));

    return 0;
}

/**
 * NOTE: If we use kprobe/do_sys_openat2, it will encounter the error: Invalid argument.
 *       It seems this is because this syscall cannot be fault injected.
 *       To know which syscall can be injected, use 'cat /proc/kallsyms | grep _eil_addr'
 */
SEC("kprobe/__x64_sys_openat")
int BPF_KPROBE(__x64_sys_openat, int dfd, const char *filename)
{
    if (!is_current_pid_or_tgid(pid))
    {
        return 0;
    }

    /**
     * NOTE: On x86-64 systems, syscalls are wrapped if
     * ARCH_HAS_SYSCALL_WRAPPER=y is set in the kernel config.
     * E.g.:
     * asmlinkage long sys_xyzzy(const struct pt_regs *regs)
     * {
     *     return SyS_xyzzy(regs->di, regs->si, regs->dx);
     * }
     * In this case, we cannot get some parameters' value,
     * just like the filename in __x64_sys_openat.
     * To get the value, we must get the raw regs pointer
     * first, then use PT_REGS_PARMn_SYSCALL or
     * PT_REGS_PARMn_CORE_SYSCALL to get the value.
     * @see https://github.com/iovisor/bcc/commit/2da34267fcae4485f4e05a17521214749f6f0edd
     *      https://github.com/libbpf/libbpf/commit/50b4d99bbc48b21fef19cb4255d41290b80f786e
     */
    struct pt_regs *new_ctx = PT_REGS_SYSCALL_REGS(ctx);
    filter_by_filename((void *)PT_REGS_PARM2_CORE_SYSCALL(new_ctx));

    return 0;
}

SEC("kprobe/__x64_sys_openat2")
int BPF_KPROBE(__x64_sys_openat2, int dfd, const char *filename)
{
    if (!is_current_pid_or_tgid(pid))
    {
        return 0;
    }

    struct pt_regs *new_ctx = PT_REGS_SYSCALL_REGS(ctx);
    filter_by_filename((void *)PT_REGS_PARM2_CORE_SYSCALL(new_ctx));

    return 0;
}

/**
 * NOTE: fd_install is called by open, openat and socket
 * which may be a good choice for recording. But for now,
 * recording open, openat seperately seems fine for this
 * project.
 * @see https://github.com/ShiftLeftSecurity/traceleft/blob/master/documentation/file-tracking.md
 */
/*
SEC("kprobe/fd_install")
int BPF_KPROBE(fd_install, unsigned int fd, struct file *file)
{
    if (!is_current_pid_or_tgid(pid))
    {
        return 0;
    }
    bpf_printk("fd_install %d", fd);

    struct path path;
    struct dentry *dentry;
    struct qstr pathname;
    char filename[256];

    bpf_probe_read(&path, sizeof(path), (const void *)&file->f_path);

    dentry = path.dentry;
    bpf_probe_read(&pathname, sizeof(pathname), (const void *)&dentry->d_name);
    bpf_probe_read_str((void *)filename, sizeof(filename), (const void *)pathname.name);

    bpf_printk("File: %s", filename);
    return 0;
}
*/

SEC("kretprobe/__x64_sys_open")
int BPF_KRETPROBE(__x64_sys_open_exit, long ret)
{

    if (!is_current_pid_or_tgid(pid))
    {
        return 0;
    }

    record_fd(ret);

    return 0;
}

SEC("kretprobe/__x64_sys_openat")
int BPF_KRETPROBE(__x64_sys_openat_exit, long ret)
{
    if (!is_current_pid_or_tgid(pid))
    {
        return 0;
    }

    record_fd(ret);

    return 0;
}

SEC("kretprobe/__x64_sys_openat2")
int BPF_KRETPROBE(__x64_sys_openat2_exit, long ret)
{
    if (!is_current_pid_or_tgid(pid))
    {
        return 0;
    }

    record_fd(ret);

    return 0;
}

SEC("kretprobe/__x64_sys_socket")
int BPF_KRETPROBE(__x64_sys_socket_exit, long ret)
{
    if (!is_current_pid_or_tgid(pid))
    {
        return 0;
    }
    bpf_printk("create socket %d", ret);
    volatile int fd_index = (unsigned long)ret % FD_SIZE;
    int b = fd_index;
    if (b >= 0 && b < FD_SIZE)
    {
        record_fds[b] = 1;
        bpf_printk("record fd %d", ret);
    }
    return 0;
}

long static inline loop_fn(u32 index, void *ctx)
{
    bpf_printk(".");
    return 0;
}

int static inline handle_events(struct pt_regs *ctx, bool is_send, int length)
{
    struct pt_regs *new_ctx = PT_REGS_SYSCALL_REGS(ctx);
    unsigned int fd = PT_REGS_PARM1_CORE_SYSCALL(new_ctx);
    bool fault_injected = false;
    if (fd >= 0 && fd < FD_SIZE && record_fds[fd])
    {
        unsigned int b = fuzz_bit_index;
        if (get_bit(fuzz_bytes, FUZZ_BYTES_SIZE, fuzz_bit_index++))
        {
            fault_injected = true;
            if (get_bit(fuzz_bytes, FUZZ_BYTES_SIZE, fuzz_bit_index++))
            {
                bpf_printk("inject fault to fd %d.", fd);
                bpf_override_return(ctx, -1);
            }
            else
            {
                bpf_printk("inject delay to fd %d.", fd);
                /**
                 * in our env, 0xfff ~= 1ms
                 */
                bpf_loop(0xfff * 50, loop_fn, &ctx, 0);
            }
        }
    }

    record_coverage(is_send, length, fd, fault_injected);

    return 0;
}

/**
 * write syscalls: write, writev, pwritev, pwritev2
 */
SEC("kprobe/__x64_sys_write")
int BPF_KPROBE(sys_write, int fd, const void *buf, size_t count)
{
    if (!is_current_pid_or_tgid(pid))
    {
        return 0;
    }
    return handle_events(ctx, true, count);
}

SEC("kprobe/__x64_sys_writev")
int BPF_KPROBE(sys_writev, int fd, const struct iovec *iov, int iovcnt)
{
    if (!is_current_pid_or_tgid(pid))
    {
        return 0;
    }
    return handle_events(ctx, true, get_length_from_iovec(iov, iovcnt));
}

SEC("kprobe/__x64_sys_pwritev")
int BPF_KPROBE(sys_pwritev, int fd, const struct iovec *iov, int iovcnt)
{
    if (!is_current_pid_or_tgid(pid))
    {
        return 0;
    }
    return handle_events(ctx, true, get_length_from_iovec(iov, iovcnt));
}

SEC("kprobe/__x64_sys_pwritev2")
int BPF_KPROBE(sys_pwritev2, int fd, const struct iovec *iov, int iovcnt)
{
    if (!is_current_pid_or_tgid(pid))
    {
        return 0;
    }
    return handle_events(ctx, true, get_length_from_iovec(iov, iovcnt));
}

/**
 * read syscalls: read, readv, preadv, preadv2
 */
SEC("kprobe/__x64_sys_read")
int BPF_KPROBE(sys_read, int fd, void *buf, size_t count)
{
    if (!is_current_pid_or_tgid(pid))
    {
        return 0;
    }
    return handle_events(ctx, false, count);
}

SEC("kprobe/__x64_sys_readv")
int BPF_KPROBE(sys_readv, int fd, const struct iovec *iov, int iovcnt)
{
    if (!is_current_pid_or_tgid(pid))
    {
        return 0;
    }
    return handle_events(ctx, false, get_length_from_iovec(iov, iovcnt));
}

SEC("kprobe/__x64_sys_preadv")
int BPF_KPROBE(sys_preadv, int fd, const struct iovec *iov, int iovcnt)
{
    if (!is_current_pid_or_tgid(pid))
    {
        return 0;
    }
    return handle_events(ctx, false, get_length_from_iovec(iov, iovcnt));
}

SEC("kprobe/__x64_sys_preadv2")
int BPF_KPROBE(sys_preadv2, int fd, const struct iovec *iov, int iovcnt)
{
    if (!is_current_pid_or_tgid(pid))
    {
        return 0;
    }
    return handle_events(ctx, false, get_length_from_iovec(iov, iovcnt));
}

/**
 * recv syscalls: recv, recvfrom, recvmsg, recvmmsg
 */
SEC("kprobe/__x64_sys_recv")
int BPF_KPROBE(sys_recv, int sockfd, void *buf, size_t len)
{
    if (!is_current_pid_or_tgid(pid))
    {
        return 0;
    }
    return handle_events(ctx, false, len);
}

SEC("kprobe/__x64_sys_recvfrom")
int BPF_KPROBE(sys_recvfrom, int sockfd, void *buf, size_t len)
{
    if (!is_current_pid_or_tgid(pid))
    {
        return 0;
    }
    return handle_events(ctx, false, len);
}

SEC("kprobe/__x64_sys_recvmsg")
int BPF_KPROBE(sys_recvmsg, int sockfd, struct msghdr *msg)
{
    if (!is_current_pid_or_tgid(pid))
    {
        return 0;
    }
    struct msghdr copied_msg;
    bpf_probe_read(&copied_msg, sizeof(copied_msg), msg);
    return handle_events(ctx, false, copied_msg.msg_iter.count);
}

SEC("kprobe/__x64_sys_recvmmsg")
int BPF_KPROBE(sys_recvmmsg, int sockfd, struct mmsghdr *msg)
{
    if (!is_current_pid_or_tgid(pid))
    {
        return 0;
    }
    struct mmsghdr copied_msg;
    bpf_probe_read(&copied_msg, sizeof(copied_msg), msg);
    return handle_events(ctx, false, copied_msg.msg_len);
}

/**
 * send syscalls: send, sendto, sendmsg, sendmmsg
 */
SEC("kprobe/__x64_sys_send")
int BPF_KPROBE(sys_send, int sockfd, const void *buf, size_t len)
{
    if (!is_current_pid_or_tgid(pid))
    {
        return 0;
    }
    return handle_events(ctx, true, len);
}

SEC("kprobe/__x64_sys_sendto")
int BPF_KPROBE(sys_sendto, int sockfd, const void *buf, size_t len)
{
    if (!is_current_pid_or_tgid(pid))
    {
        return 0;
    }
    return handle_events(ctx, true, len);
}

SEC("kprobe/__x64_sys_sendmsg")
int BPF_KPROBE(sys_sendmsg, int sockfd, struct msghdr *msg)
{
    if (!is_current_pid_or_tgid(pid))
    {
        return 0;
    }
    struct msghdr copied_msg;
    bpf_probe_read(&copied_msg, sizeof(copied_msg), msg);
    return handle_events(ctx, true, copied_msg.msg_iter.count);
}

SEC("kprobe/__x64_sys_sendmmsg")
int BPF_KPROBE(sys_sendmmsg, int sockfd, struct mmsghdr *msg)
{
    if (!is_current_pid_or_tgid(pid))
    {
        return 0;
    }
    struct mmsghdr copied_msg;
    bpf_probe_read(&copied_msg, sizeof(copied_msg), msg);
    return handle_events(ctx, true, copied_msg.msg_len);
}

uint64_t get_length_from_iovec(const struct iovec *iov, int iovcnt)
{
    uint64_t length = 0;
    // for (uint64_t i = 0; i < iovcnt; i++) {
    // 	struct iovec copied_iov;
    //     bpf_probe_read(&copied_iov, sizeof(struct iovec), iov + sizeof(struct iovec) * i);
    // 	length += copied_iov.iov_len;
    // }

    struct iovec copied_iov;
    bpf_probe_read(&copied_iov, sizeof(struct iovec), iov);
    length += copied_iov.iov_len;

    if (iovcnt > 1)
    {
        bpf_printk("iov not read, iovcnt = %d", iovcnt);
    }

    return length;
}

char LICENSE[] SEC("license") = "Dual BSD/GPL";