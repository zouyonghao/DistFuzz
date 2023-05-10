#include "vmlinux.h"
#include <bpf/bpf_core_read.h>
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>
// #include <linux/errno.h>
#include "utils.h"
#include <linux/errno.h>
#include <linux/limits.h>

#define SHOULD_RECORD_SIZE 103

#define FD_SIZE 103

#define FUZZ_BYTES_SIZE 1024 * 1024

int pid = 0;

/**
 * this is used for determining whether the file
 * descriptor should be recorded created by open
 * and openat.
 */
unsigned char should_record[SHOULD_RECORD_SIZE];

/* this records filtered files and sockets */
unsigned long record_fds[FD_SIZE];

unsigned char fuzz_bytes[FUZZ_BYTES_SIZE];

unsigned volatile int fuzz_bit_index = 0;

static inline int is_current_pid_or_tgid(int pid)
{
    u32 current_pid = bpf_get_current_pid_tgid() >> 32;
    u32 current_tgid = bpf_get_current_pid_tgid();

    if (current_pid != pid && current_tgid != pid)
    {
        return 0;
    }
    return 1;
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
    char fname[256];
    bpf_probe_read(&fname, sizeof(fname), (void *)PT_REGS_PARM2_CORE_SYSCALL(new_ctx));

    if (str_contains(fname, ".o", sizeof(fname), 2) == 0)
    {
        return 0;
    }
    if (str_contains(fname, ".so", sizeof(fname), 3) == 0)
    {
        return 0;
    }
    if (str_contains(fname, ".cfg", sizeof(fname), 4) == 0)
    {
        return 0;
    }
    if (str_contains(fname, "lib", sizeof(fname), 3) == 0)
    {
        return 0;
    }
    if (str_contains(fname, "/sys", sizeof(fname), 4) == 0)
    {
        return 0;
    }
    if (str_contains(fname, "/proc", sizeof(fname), 5) == 0)
    {
        return 0;
    }
    if (str_contains(fname, "/dev", sizeof(fname), 4) == 0)
    {
        return 0;
    }
    if (str_contains(fname, ".pro", sizeof(fname), 4) == 0)
    {
        return 0;
    }
    if (str_contains(fname, ".jar", sizeof(fname), 4) == 0)
    {
        return 0;
    }
    if (str_contains(fname, "/var", sizeof(fname), 4) == 0)
    {
        return 0;
    }
    if (str_contains(fname, "tmp", sizeof(fname), 3) == 0)
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
    /** TODO: use logic in openat */
    return 0;
}

SEC("kretprobe/__x64_sys_openat")
int BPF_KRETPROBE(__x64_sys_openat_exit, long ret)
{
    if (!is_current_pid_or_tgid(pid))
    {
        return 0;
    }

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
            if (b >= 0 && b < FD_SIZE)
            {
                record_fds[b] = 1;
                bpf_printk("record fd %d", ret);
            }
        }
    }

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

int static inline handle_events(struct pt_regs *ctx)
{
    if (!is_current_pid_or_tgid(pid))
    {
        return 0;
    }
    struct pt_regs *new_ctx = PT_REGS_SYSCALL_REGS(ctx);
    unsigned int fd = PT_REGS_PARM1_CORE_SYSCALL(new_ctx);
    if (fd >= 0 && fd < FD_SIZE && record_fds[fd])
    {
        unsigned int b = fuzz_bit_index;
        if (get_bit(fuzz_bytes, FUZZ_BYTES_SIZE, fuzz_bit_index++))
        {
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
                for (unsigned long tmp = 0; tmp < 0xfff; tmp++)
                {
                    bpf_printk(".");
                }
            }
        }
    }
    return 0;
}

/**
 * write syscalls: write, writev, pwritev, pwritev2
 */
SEC("kprobe/__x64_sys_write")
int BPF_KPROBE(sys_write) { return handle_events(ctx); }

SEC("kprobe/__x64_sys_writev")
int BPF_KPROBE(sys_writev) { return handle_events(ctx); }

SEC("kprobe/__x64_sys_pwritev")
int BPF_KPROBE(sys_pwritev) { return handle_events(ctx); }

SEC("kprobe/__x64_sys_pwritev2")
int BPF_KPROBE(sys_pwritev2) { return handle_events(ctx); }

/**
 * read syscalls: read, readv, preadv, preadv2
 */
SEC("kprobe/__x64_sys_read")
int BPF_KPROBE(sys_read) { return handle_events(ctx); }

SEC("kprobe/__x64_sys_readv")
int BPF_KPROBE(sys_readv) { return handle_events(ctx); }

SEC("kprobe/__x64_sys_preadv")
int BPF_KPROBE(sys_preadv) { return handle_events(ctx); }

SEC("kprobe/__x64_sys_preadv2")
int BPF_KPROBE(sys_preadv2) { return handle_events(ctx); }

/**
 * recv syscalls: recv, recvfrom, recvmsg, recvmmsg
 */
SEC("kprobe/__x64_sys_recv")
int BPF_KPROBE(sys_recv) { return handle_events(ctx); }

SEC("kprobe/__x64_sys_recvfrom")
int BPF_KPROBE(sys_recvfrom) { return handle_events(ctx); }

SEC("kprobe/__x64_sys_recvmsg")
int BPF_KPROBE(sys_recvmsg) { return handle_events(ctx); }

SEC("kprobe/__x64_sys_recvmmsg")
int BPF_KPROBE(sys_recvmmsg) { return handle_events(ctx); }

/**
 * send syscalls: send, sendto, sendmsg, sendmmsg
 */
SEC("kprobe/__x64_sys_send")
int BPF_KPROBE(sys_send) { return handle_events(ctx); }

SEC("kprobe/__x64_sys_sendto")
int BPF_KPROBE(sys_sendto) { return handle_events(ctx); }

SEC("kprobe/__x64_sys_sendmsg")
int BPF_KPROBE(sys_sendmsg) { return handle_events(ctx); }

SEC("kprobe/__x64_sys_sendmmsg")
int BPF_KPROBE(sys_sendmmsg) { return handle_events(ctx); }

char LICENSE[] SEC("license") = "Dual BSD/GPL";