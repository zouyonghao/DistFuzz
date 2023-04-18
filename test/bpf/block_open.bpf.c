
#include "vmlinux.h"
#include <bpf/bpf_core_read.h>
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>
// #include <linux/errno.h>
// #include "buildins.h"

#define memcpy(dest, src, n) __builtin_memcpy((dest), (src), (n))
#define memcmp(dest, src, n) __builtin_memcmp((dest), (src), (n))

#define FILE_NAME "/tmp/secret"

static __always_inline int str_equal(const char *cs, const char *ct, int size)
{
    int len = 0;
    unsigned char c1, c2;
    for (len = 0; len < (size & 0xffff); len++)
    {
        c1 = *cs++;
        c2 = *ct++;
        if (c1 != c2)
            return c1 < c2 ? -1 : 1;
        if (!c1)
            break;
    }
    return 0;
}

static int max(int a, int b) { return a > b ? a : b; }

SEC("kprobe/do_sys_openat2")
int BPF_KPROBE(do_sys_openat2, int dfd, const char *filename)
{
    char fname[256];
    bpf_probe_read_str(&fname, sizeof(fname), (void *)filename);
    if (str_equal(fname, FILE_NAME, sizeof(fname)) == 0)
    {
        bpf_printk("Blocking open of %s\n", fname);
        return -1;
    }
    // bpf_printk("Blocking open of %s\n", filename);
    return 0;
}

char LICENSE[] SEC("license") = "Dual BSD/GPL";