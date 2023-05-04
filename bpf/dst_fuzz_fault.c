#include "dst_fuzz_fault.skel.h"
#include <bpf/libbpf.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/resource.h>
#include <unistd.h>

static int libbpf_print_fn(enum libbpf_print_level level, const char *format, va_list args)
{
    return vfprintf(stderr, format, args);
}

static volatile sig_atomic_t stop;

static void sig_int(int signo) { stop = 1; }

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("Usage: sudo ./dst_fuzz_fault test_program [args...]\n");
        return 0;
    }
    struct dst_fuzz_fault_bpf *skel;
    int err;

    /* Set up libbpf errors and debug info callback */
    libbpf_set_print(libbpf_print_fn);

    /* Open load and verify BPF application */
    skel = dst_fuzz_fault_bpf__open_and_load();
    if (!skel)
    {
        fprintf(stderr, "Failed to open BPF skeleton\n");
        return 1;
    }

    /* Attach tracepoint handler */
    err = dst_fuzz_fault_bpf__attach(skel);
    if (err)
    {
        fprintf(stderr, "Failed to attach BPF skeleton\n");
        goto cleanup;
    }

    if (signal(SIGINT, sig_int) == SIG_ERR)
    {
        fprintf(stderr, "can't set signal handler: %s\n", strerror(errno));
        goto cleanup;
    }

    printf("Successfully started! Please run `sudo cat /sys/kernel/debug/tracing/trace_pipe` "
           "to see output of the BPF programs.\n");

    /* Start the target program */
    pid_t child_pid = fork();
    if (child_pid == 0)
    {
        // skel->bss->pid = getpid();
        fprintf(stderr, "new pid is %d\n", getpid());
        if (execv(argv[1], &argv[1]))
        {
            perror("execve failed");
            return -1;
        }
    }

    skel->bss->pid = child_pid;
    fprintf(stderr, "pid is %d\n", child_pid);
    while (!stop)
    {
        // fprintf(stderr, ".");
        sleep(1);
    }

cleanup:
    dst_fuzz_fault_bpf__destroy(skel);
    return -err;
}
