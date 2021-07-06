1. failed to start after killing

    ```
    1: starting
    2: raft_start(): io: last entry on disk has index 389, which is behind last snapshot's index 345408
    2: stopping
    1: raft_start(): io: last entry on disk has index 389, which is behind last snapshot's index 345408
    1: stopping
    ```

    https://github.com/canonical/raft/issues/199

2. AddressSanitizer:DEADLYSIGNAL
    ```
    AddressSanitizer:DEADLYSIGNAL
    =================================================================
    ==81620==ERROR: AddressSanitizer: SEGV on unknown address 0x000000000009 (pc 0x7f23fcb095c3 bp 0x7fff95ea3750 sp 0x7fff95ea35b0 T0)
    ==81620==The signal is caused by a READ memory access.
    ==81620==Hint: address points to the zero page.
        #0 0x7f23fcb095c2 in tickLeader /home/zyh/raft/src/tick.c:184:46
        #1 0x7f23fcb08bce in tick /home/zyh/raft/src/tick.c:214:18
        #2 0x7f23fcb08858 in tickCb /home/zyh/raft/src/tick.c:226:10
        #3 0x7f23fcb1aaaa in uvTickTimerCb /home/zyh/raft/src/uv.c:135:9
        #4 0x7f23fcf83907 in uv__run_timers (/usr/lib/x86_64-linux-gnu/libuv.so.1+0x18907)
        #5 0x7f23fcf76c34 in uv_run (/usr/lib/x86_64-linux-gnu/libuv.so.1+0xbc34)
        #6 0x5133be in main /home/zyh/raft/example/server.c:469:10
        #7 0x7f23fbd17bf6 in __libc_start_main /build/glibc-S9d2JN/glibc-2.27/csu/../csu/libc-start.c:310
        #8 0x41a8a9 in _start (/home/zyh/raft/example/server+0x41a8a9)

    AddressSanitizer can not provide additional info.
    SUMMARY: AddressSanitizer: SEGV /home/zyh/raft/src/tick.c:184:46 in tickLeader
    ==81620==ABORTING

    ```

3. AddressSanitizer: bad-free

    ```
    ==24124==WARNING: AddressSanitizer failed to allocate 0x312e302e302e3732 bytes
    =================================================================
    ==24124==ERROR: AddressSanitizer: attempting free on address which was not malloc()-ed: 0x611000001618 in thread T0
        #0 0x4934fd  (/home/zyh/raft/example/server+0x4934fd)
        #1 0x7fb7421e92dc  (/home/zyh/raft/.libs/libraft.so.0+0xe2dc) src/heap.c:65
        #2 0x7fb742205bba  (/home/zyh/raft/.libs/libraft.so.0+0x2abba) src/uv_encoding.c:371
        #3 0x7fb74220cb5a  (/home/zyh/raft/.libs/libraft.so.0+0x31b5a) src/uv_recv.c:260
        #4 0x7fb742c7e5ce  (/usr/lib/x86_64-linux-gnu/libuv.so.1+0x155ce)
        #5 0x7fb742c7f33b  (/usr/lib/x86_64-linux-gnu/libuv.so.1+0x1633b)
        #6 0x7fb742c8433f  (/usr/lib/x86_64-linux-gnu/libuv.so.1+0x1b33f)
        #7 0x7fb742c74cc7  (/usr/lib/x86_64-linux-gnu/libuv.so.1+0xbcc7)
        #8 0x4c3e4f  (/home/zyh/raft/example/server+0x4c3e4f)
        #9 0x7fb74142abf6  (/lib/x86_64-linux-gnu/libc.so.6+0x21bf6)
        #10 0x41b879  (/home/zyh/raft/example/server+0x41b879)

    0x611000001618 is located 152 bytes inside of 200-byte region [0x611000001580,0x611000001648)
    allocated by thread T0 here:
        #0 0x49377d  (/home/zyh/raft/example/server+0x49377d)
        #1 0x7fb74220c2c9  (/home/zyh/raft/.libs/libraft.so.0+0x312c9) src/uv_recv.c:347

    SUMMARY: AddressSanitizer: bad-free (/home/zyh/raft/example/server+0x4934fd)
    ==24124==ABORTING
    ```