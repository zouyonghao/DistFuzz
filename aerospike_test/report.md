1. CRITICAL log, error case 202

    ```
    Jun 07 2021 16:48:02 GMT: WARNING (fabric): (fabric.c:747) no IPv4 addresses configured for fabric
    Jun 07 2021 16:48:02 GMT: CRITICAL (fabric): (fabric.c:373) error creating fabric published endpoint list
    Jun 07 2021 16:48:02 GMT: WARNING (as): (signal.c:218) SIGUSR1 received, aborting Aerospike Community Edition build 5.6.0.4 os ubuntu18.04
    Jun 07 2021 16:48:02 GMT: WARNING (as): (log.c:630) stacktrace: registers: rax 0000000000000000 rbx 000000000000000a rcx 00007f90bf29b817 rdx 0000000000000000 rsi 00007ffca6402690 rdi 0000000000000002 rbp 0000000000000003 rsp 00007ffca6402690 r8 0000000000000000 r9 00007ffca6402690 r10 0000000000000008 r11 0000000000000246 r12 0000000000000004 r13 0000000000000004 r14 0000000000000a00 r15 0000557420a0f9e8 rip 00007f90bf29b817
    Jun 07 2021 16:48:02 GMT: WARNING (as): (log.c:643) stacktrace: found 10 frames: 0x18cd64 0xd1f1b 0x7f90bf29b980 0x7f90bf29b817 0x18c76f 0x11d517 0x55275 0x53cf3 0x7f90bdf51bf7 0x5445a offset 0x55742046f000
    Jun 07 2021 16:48:02 GMT: WARNING (as): (log.c:653) stacktrace: frame 0: /home/zyh/aerospike-server/target/Linux-x86_64/bin/asd(cf_log_stack_trace+0x115) [0x5574205fbd64]
    Jun 07 2021 16:48:02 GMT: WARNING (as): (log.c:653) stacktrace: frame 1: /home/zyh/aerospike-server/target/Linux-x86_64/bin/asd(as_sig_handle_usr1+0x72) [0x557420540f1b]
    Jun 07 2021 16:48:02 GMT: WARNING (as): (log.c:653) stacktrace: frame 2: /lib/x86_64-linux-gnu/libpthread.so.0(+0x12980) [0x7f90bf29b980]
    Jun 07 2021 16:48:02 GMT: WARNING (as): (log.c:653) stacktrace: frame 3: /lib/x86_64-linux-gnu/libpthread.so.0(raise+0xc7) [0x7f90bf29b817]
    Jun 07 2021 16:48:02 GMT: WARNING (as): (log.c:653) stacktrace: frame 4: /home/zyh/aerospike-server/target/Linux-x86_64/bin/asd(cf_log_write_no_return+0x93) [0x5574205fb76f]
    Jun 07 2021 16:48:02 GMT: WARNING (as): (log.c:653) stacktrace: frame 5: /home/zyh/aerospike-server/target/Linux-x86_64/bin/asd(as_fabric_init+0x223) [0x55742058c517]
    Jun 07 2021 16:48:02 GMT: WARNING (as): (log.c:653) stacktrace: frame 6: /home/zyh/aerospike-server/target/Linux-x86_64/bin/asd(as_run+0x444) [0x5574204c4275]
    Jun 07 2021 16:48:02 GMT: WARNING (as): (log.c:653) stacktrace: frame 7: /home/zyh/aerospike-server/target/Linux-x86_64/bin/asd(main+0x9) [0x5574204c2cf3]
    Jun 07 2021 16:48:02 GMT: WARNING (as): (log.c:653) stacktrace: frame 8: /lib/x86_64-linux-gnu/libc.so.6(__libc_start_main+0xe7) [0x7f90bdf51bf7]
    Jun 07 2021 16:48:02 GMT: WARNING (as): (log.c:653) stacktrace: frame 9: /home/zyh/aerospike-server/target/Linux-x86_64/bin/asd(_start+0x2a) [0x5574204c345a]
    ```