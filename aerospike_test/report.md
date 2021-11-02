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

    issues
    * https://github.com/aerospike/aerospike-server/issues/27

2. Read old data?

    test_cases/1547
    
    issues
    * https://github.com/aerospike/aerospike-server/issues/30

    stale read
    community version does not support this, while enterprise version does.

3. stack-buffer-underflow

    ```
    =================================================================
    ==20195==ERROR: AddressSanitizer: stack-buffer-underflow on address 0x7ffc00dd503f at pc 0x000000a83a3a bp 0x7ffc00dd5010 sp 0x7ffc00dd5008
    WRITE of size 1 at 0x7ffc00dd503f thread T0
        #0 0xa83a39  (/home/zyh/aerospike-server/target/Linux-x86_64/bin/asd+0xa83a39) cf/src/hardware.c:481
        #1 0xa81f31  (/home/zyh/aerospike-server/target/Linux-x86_64/bin/asd+0xa81f31) cf/src/hardware.c:501
        #2 0xa782d3  (/home/zyh/aerospike-server/target/Linux-x86_64/bin/asd+0xa782d3) cf/src/hardware.c:742
        #3 0xa764f4  (/home/zyh/aerospike-server/target/Linux-x86_64/bin/asd+0xa764f4) cf/src/hardware.c:1911
        #4 0x506555  (/home/zyh/aerospike-server/target/Linux-x86_64/bin/asd+0x506555) as/src/base/as.c:273
        #5 0x86f0b4  (/home/zyh/aerospike-server/target/Linux-x86_64/bin/asd+0x86f0b4) as/src/base/main.c:37
        #6 0x7fdfb3449bf6  (/lib/x86_64-linux-gnu/libc.so.6+0x21bf6)
        #7 0x459229  (/home/zyh/aerospike-server/target/Linux-x86_64/bin/asd+0x459229)

    Address 0x7ffc00dd503f is located in stack of thread T0 at offset 31 in frame
        #0 0xa8369f  (/home/zyh/aerospike-server/target/Linux-x86_64/bin/asd+0xa8369f)

    This frame has 3 object(s):
        [32, 132) 'buff' (line 473) <== Memory access at offset 31 underflows this variable
        [176, 184) 'limit' (line 474)
        [208, 216) 'end' (line 485)
    HINT: this may be a false positive if your program uses some custom stack unwind mechanism, swapcontext or vfork
        (longjmp and C++ exceptions *are* supported)
    SUMMARY: AddressSanitizer: stack-buffer-underflow (/home/zyh/aerospike-server/target/Linux-x86_64/bin/asd+0xa83a39)
    Shadow bytes around the buggy address:
    0x1000001b29b0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
    0x1000001b29c0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
    0x1000001b29d0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
    0x1000001b29e0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
    0x1000001b29f0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
    =>0x1000001b2a00: 00 00 00 00 f1 f1 f1[f1]00 00 00 00 00 00 00 00
    0x1000001b2a10: 00 00 00 00 04 f2 f2 f2 f2 f2 00 f2 f2 f2 f8 f3
    0x1000001b2a20: f3 f3 f3 f3 00 00 00 00 00 00 00 00 00 00 00 00
    0x1000001b2a30: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
    0x1000001b2a40: 00 00 00 00 00 00 00 00 f1 f1 f1 f1 00 f3 f3 f3
    0x1000001b2a50: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
    Shadow byte legend (one shadow byte represents 8 application bytes):
    Addressable:           00
    Partially addressable: 01 02 03 04 05 06 07
    Heap left redzone:       fa
    Freed heap region:       fd
    Stack left redzone:      f1
    Stack mid redzone:       f2
    Stack right redzone:     f3
    Stack after return:      f5
    Stack use after scope:   f8
    Global redzone:          f9
    Global init order:       f6
    Poisoned by user:        f7
    Container overflow:      fc
    Array cookie:            ac
    Intra object redzone:    bb
    ASan internal:           fe
    Left alloca redzone:     ca
    Right alloca redzone:    cb
    Shadow gap:              cc
    ==20195==ABORTING
    ```