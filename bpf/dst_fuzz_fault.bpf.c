
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
 * NOTE: If we use kprobe/do_sys_openat2, it will encounter the error: Invalid argument.
 *       It seems this is because this syscall cannot be fault injected.
 *       To know which syscall can be injected, use 'cat /proc/kallsyms | grep _eil_addr'
 */
SEC("kprobe/__x64_sys_openat")
int BPF_KPROBE(__x64_sys_openat, int dfd, const char *filename)
{
    u32 current_pid = bpf_get_current_pid_tgid() >> 32;
    u32 current_tgid = bpf_get_current_pid_tgid();

    if (current_pid == pid || current_tgid == pid)
    {
        bpf_printk("current pid = %d\n", pid);
        return 0;
    }

    // if (current_pid != pid && current_tgid != pid)
    // {
    //     return 0;
    // }
    // bpf_printk("current pid = %d\n", pid);
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
        bpf_printk("opening %s\n", fname);
        bpf_printk("current_pid = %d, current_tgid = %d, pid = %d\n", current_pid, current_tgid, pid);
        return 0;
    }
    if (str_contains(fname, ".so", sizeof(fname), 2) == 0)
    {
        return 0;
    }
    return 0;
}

/**
 * write syscalls: write, writev, pwritev, pwritev2
 */
SEC("kprobe/__x64_sys_write")
int BPF_KPROBE(sys_write, unsigned int fd)
{
    // char *file_path;
    // if (get_file_path(fd, &file_path) != -1)
    // {
    //     bpf_printk("Blocking open of %s\n", file_path);
    // }
    return 0;
}

SEC("kprobe/__x64_sys_writev")
int BPF_KPROBE(sys_writev, unsigned int fd) { return 0; }

SEC("kprobe/__x64_sys_pwritev")
int BPF_KPROBE(sys_pwritev, unsigned int fd) { return 0; }

SEC("kprobe/__x64_sys_pwritev2")
int BPF_KPROBE(sys_pwritev2, unsigned int fd) { return 0; }

/**
 * read syscalls: read, readv, preadv, preadv2
 */
SEC("kprobe/__x64_sys_read")
int BPF_KPROBE(sys_read, unsigned int fd) { return 0; }

SEC("kprobe/__x64_sys_readv")
int BPF_KPROBE(sys_readv, unsigned int fd) { return 0; }

SEC("kprobe/__x64_sys_preadv")
int BPF_KPROBE(sys_preadv, unsigned int fd) { return 0; }

SEC("kprobe/__x64_sys_preadv2")
int BPF_KPROBE(sys_preadv2, unsigned int fd) { return 0; }

/**
 * recv syscalls: recv, recvfrom, recvmsg, recvmmsg
 */
SEC("kprobe/__x64_sys_recv")
int BPF_KPROBE(sys_recv, unsigned int fd) { return 0; }

SEC("kprobe/__x64_sys_recvfrom")
int BPF_KPROBE(sys_recvfrom, unsigned int fd) { return 0; }

SEC("kprobe/__x64_sys_recvmsg")
int BPF_KPROBE(sys_recvmsg, unsigned int fd) { return 0; }

SEC("kprobe/__x64_sys_recvmmsg")
int BPF_KPROBE(sys_recvmmsg, unsigned int fd) { return 0; }

/**
 * send syscalls: send, sendto, sendmsg, sendmmsg
 */
SEC("kprobe/__x64_sys_send")
int BPF_KPROBE(sys_send, unsigned int fd) { return 0; }

SEC("kprobe/__x64_sys_sendto")
int BPF_KPROBE(sys_sendto, unsigned int fd) { return 0; }

SEC("kprobe/__x64_sys_sendmsg")
int BPF_KPROBE(sys_sendmsg, unsigned int fd) { return 0; }

SEC("kprobe/__x64_sys_sendmmsg")
int BPF_KPROBE(sys_sendmmsg, unsigned int fd) { return 0; }

char LICENSE[] SEC("license") = "Dual BSD/GPL";