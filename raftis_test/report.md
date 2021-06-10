1. 
    ```
                member 0 : 127.0.0.1:8901
                member 1 : 127.0.0.1:8902
                member 2 : 127.0.0.1:8903
                    local_ip : 127.0.0.1
                local_port : 8901
                        path : ./data1/
            check_leader_us : 7779329
                heartbeat_us : 3000000
    append_entries_size_once : 10240000
    append_entries_count_once : 102400
                single_mode : false
    AddressSanitizer:DEADLYSIGNAL
    =================================================================
    ==22633==ERROR: AddressSanitizer: SEGV on unknown address 0x000000000000 (pc 0x000000540a5b bp 0x7fffe3757db0 sp 0x7fffe3757cb0 T0)
    ==22633==The signal is caused by a READ memory access.
    ==22633==Hint: address points to the zero page.
        #0 0x540a5a in floyd::FloydImpl::~FloydImpl() /root/floyd/floyd/src/floyd_impl.cc:159:3
        #1 0x541000 in floyd::FloydImpl::~FloydImpl() /root/floyd/floyd/src/floyd_impl.cc:157:25
        #2 0x547e5f in floyd::Floyd::Open(floyd::Options const&, floyd::Floyd**) /root/floyd/floyd/src/floyd_impl.cc:376:5
        #3 0x422f89 in main /root/floyd/floyd/example/redis/raftis.cc:133
        #4 0x7fdb3b636f44 in __libc_start_main /build/eglibc-xkFqqE/eglibc-2.19/csu/libc-start.c:287
        #5 0x495a39 in _start (/root/floyd/floyd/example/redis/output/bin/raftis+0x495a39)

    AddressSanitizer can not provide additional info.
    SUMMARY: AddressSanitizer: SEGV /root/floyd/floyd/src/floyd_impl.cc:159:3 in floyd::FloydImpl::~FloydImpl()
    ==22633==ABORTING
    ```

2.
    ```
    AddressSanitizer:DEADLYSIGNAL
    =================================================================
    ==17891==ERROR: AddressSanitizer: SEGV on unknown address 0x000000000000 (pc 0x0000005a27e6 bp 0x7ffcd0df8100 sp 0x7ffcd0df80e0 T0)
    ==17891==The signal is caused by a READ memory access.
    ==17891==Hint: address points to the zero page.
        #0 0x5a27e5  (/root/floyd/floyd/example/redis/output/bin/raftis+0x5a27e5) /root/floyd/floyd/third/pink/pink/include/bg_thread.h:?
        #1 0x544d39  (/root/floyd/floyd/example/redis/output/bin/raftis+0x544d39) /root/floyd/floyd/src/floyd_impl.cc:161
        #2 0x545270  (/root/floyd/floyd/example/redis/output/bin/raftis+0x545270) /root/floyd/floyd/src/floyd_impl.cc:157
        #3 0x54c0cf  (/root/floyd/floyd/example/redis/output/bin/raftis+0x54c0cf) /root/floyd/floyd/src/floyd_impl.cc:376
        #4 0x54316e  (/root/floyd/floyd/example/redis/output/bin/raftis+0x54316e)
        #5 0x7f8f12d3ef44  (/lib/x86_64-linux-gnu/libc.so.6+0x21f44)
        #6 0x495709  (/root/floyd/floyd/example/redis/output/bin/raftis+0x495709)

    AddressSanitizer can not provide additional info.
    SUMMARY: AddressSanitizer: SEGV (/root/floyd/floyd/example/redis/output/bin/raftis+0x5a27e5) 
    ==17891==ABORTING
    ```