1. failed to start after killing

    ```
    1: starting
    2: raft_start(): io: last entry on disk has index 389, which is behind last snapshot's index 345408
    2: stopping
    1: raft_start(): io: last entry on disk has index 389, which is behind last snapshot's index 345408
    1: stopping
    ```

    https://github.com/canonical/raft/issues/199

    fixed.

    https://github.com/canonical/raft/pull/218
    
    ```
    Node receives snapshot install and deletes all closed segments
    preceding the snapshot.
    After installing the snapshot, the node receives AppendEntries and
    adds the entries to an open segment.
    Node gets killed.
    Node boots and UvLoad closes the open segment using start_index 1,
    ignoring the index of the snapshot that is present in the data dir.
    Node fails to start because the newly closed segment and the snapshot are
    inconsistent with a message
    raft_start(): io: last entry on disk has index 383, which is behind last snapshot's index 3072"
    ```

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

    https://github.com/canonical/raft/issues/219

    fixed.

4. heap-buffer-overflow

    ```
    =================================================================
    ==11599==ERROR: AddressSanitizer: heap-buffer-overflow on address 0x602000000250 at pc 0x7f553b99a8e3 bp 0x7ffcf8dee520 sp 0x7ffcf8dee518
    READ of size 8 at 0x602000000250 thread T0
        #0 0x7f553b99a8e2  (/home/zyh/raft/.libs/libraft.so.0+0x808e2) byte.h:133
        #1 0x7f553b99bc9b  (/home/zyh/raft/.libs/libraft.so.0+0x81c9b) uv_encoding.c:390
        #2 0x7f553b99ad61  (/home/zyh/raft/.libs/libraft.so.0+0x80d61) uv_encoding.c:477
        #3 0x7f553b9b438e  (/home/zyh/raft/.libs/libraft.so.0+0x9a38e) uv_recv:260
        #4 0x7f553c4475ce  (/usr/lib/x86_64-linux-gnu/libuv.so.1+0x155ce)
        #5 0x7f553c44833b  (/usr/lib/x86_64-linux-gnu/libuv.so.1+0x1633b)
        #6 0x7f553c44d33f  (/usr/lib/x86_64-linux-gnu/libuv.so.1+0x1b33f)
        #7 0x7f553c43dcc7  (/usr/lib/x86_64-linux-gnu/libuv.so.1+0xbcc7)
        #8 0x4c6542  (/home/zyh/raft/example/server+0x4c6542) example/server.c:473
        #9 0x7f553ab6bbf6  (/lib/x86_64-linux-gnu/libc.so.6+0x21bf6)
        #10 0x41bfe9  (/home/zyh/raft/example/server+0x41bfe9)

    0x602000000252 is located 0 bytes to the right of 2-byte region [0x602000000250,0x602000000252)
    allocated by thread T0 here:
        #0 0x49605d  (/home/zyh/raft/example/server+0x49605d)
        #1 0x7f553b94daf4  (/home/zyh/raft/.libs/libraft.so.0+0x33af4) heap.c:10
        #2 0x7f553b94d4fa  (/home/zyh/raft/.libs/libraft.so.0+0x334fa) heap.c:57
        #3 0x7f553b9b36b5  (/home/zyh/raft/.libs/libraft.so.0+0x996b5) uv_recv.c:144
        #4 0x7f553c4474a7  (/usr/lib/x86_64-linux-gnu/libuv.so.1+0x154a7)

    SUMMARY: AddressSanitizer: heap-buffer-overflow (/home/zyh/raft/.libs/libraft.so.0+0x808e2)
    Shadow bytes around the buggy address:
    0x0c047fff7ff0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
    0x0c047fff8000: fa fa fd fd fa fa 00 fa fa fa 00 07 fa fa fd fd
    0x0c047fff8010: fa fa fd fd fa fa fd fd fa fa 00 07 fa fa 00 07
    0x0c047fff8020: fa fa 00 07 fa fa 03 fa fa fa fd fd fa fa fd fd
    0x0c047fff8030: fa fa 00 07 fa fa fd fd fa fa fd fd fa fa 00 07
    =>0x0c047fff8040: fa fa fd fd fa fa 00 07 fa fa[02]fa fa fa fa fa
    0x0c047fff8050: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
    0x0c047fff8060: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
    0x0c047fff8070: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
    0x0c047fff8080: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
    0x0c047fff8090: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
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
    ==11599==ABORTING
    ```

    可能是误报（partial write fail)

5. Memory leak in uvSnapshotLoadMeta
    ```
        buf.base = HeapMalloc(buf.len);
        if (buf.base == NULL) {
            rv = RAFT_NOMEM;
            goto err_after_open;
        }

        rv = UvFsReadInto(fd, &buf, errmsg);
        if (rv != 0) {
            tracef("read %s: %s", info->filename, errmsg);
            rv = RAFT_IOERR;
            goto err_after_buf_malloc;
        }

        crc2 = byteCrc32(header + 2, sizeof header - sizeof(uint64_t) * 2, 0);
        crc2 = byteCrc32(buf.base, buf.len, crc2);

        if (crc1 != crc2) {
            ErrMsgPrintf(errmsg, "read %s: checksum mismatch", info->filename);
            rv = RAFT_CORRUPT;
            goto err_after_open;
        }

        ...

    err_after_buf_malloc:
        HeapFree(buf.base);

    err_after_open:
        close(fd);
    ```

    issues
    * https://github.com/canonical/raft/pull/249