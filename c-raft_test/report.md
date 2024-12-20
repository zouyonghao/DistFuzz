### failed to start after killing

    ```
    1: starting
    2: raft_start(): io: last entry on disk has index 389, which is behind last snapshot's index 345408
    2: stopping
    1: raft_start(): io: last entry on disk has index 389, which is behind last snapshot's index 345408
    1: stopping
    ```

    https://github.com/canonical/raft/issues/199

    **Fixed**

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

### AddressSanitizer:DEADLYSIGNAL
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

### AddressSanitizer: bad-free

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

    **Fixed.**

### heap-buffer-overflow

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

    There are two heap-buffer-overflows:
    zyh@144ed63097a7:~/raft$ addr2line -e ./.libs/libraft.so 0x81ea0
    /home/zyh/raft/src/byte.h:133
    zyh@144ed63097a7:~/raft$ addr2line -e ./.libs/libraft.so 0x4c664
    /home/zyh/raft/src/progress.c:252

### Memory leak in uvSnapshotLoadMeta
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

    **Fixed**

### Assertion `r->last_applied <= r->commit_index' failed
    ```
    server: src/replication.c:1490: int replicationApply(struct raft *): Assertion `r->last_applied <= r->commit_index' failed.
    ```

### Assertion `r->state == RAFT_FOLLOWER || r->state == RAFT_CANDIDATE'
    ```
    server: src/recv_append_entries.c:89: int recvAppendEntries(struct raft *, raft_id, const char *, const struct raft_append_entries *): Assertion `r->state == RAFT_FOLLOWER || r->state == RAFT_CANDIDATE' failed.
    ```

### Assertion `request->args.n_entries == n'
    ```
    server: src/replication.c:1138: int replicationAppend(struct raft *, const struct raft_append_entries *, raft_index *, _Bool *): Assertion `request->args.n_entries == n' failed.
    ```

### Assertion `t->read.len > 0' failed.
    ```
    LIBRAFT   1719786478761513999 src/start.c:155 starting
    LIBRAFT   1719786478761942716 src/uv_list.c:92 segment 0000000000000001-0000000000000001
    LIBRAFT   1719786478761995532 src/uv_list.c:97 ignore cluster.yaml
    LIBRAFT   1719786478762038516 src/uv_list.c:97 ignore info.yaml
    LIBRAFT   1719786478762082263 src/uv_list.c:71 ignore metadata1
    LIBRAFT   1719786478762129766 src/uv_segment.c:891 load segment 0000000000000001-0000000000000001
    LIBRAFT   1719786478762477304 src/uv.c:485 start index 1, 1 entries
    LIBRAFT   1719786478762520575 src/uv.c:487 no snapshot
    LIBRAFT   1719786478762561970 src/start.c:163 current_term:1 voted_for:0 start_index:1 n_entries:1
    LIBRAFT   1719786478762613587 src/start.c:201 restore 1 entries starting at 1
    LIBRAFT   1719786478762743986 src/configuration.c:342 restore most recent configuration
    LIBRAFT   1719786478762786189 src/configuration.c:343 === CONFIG START ===
    LIBRAFT   1719786478762829543 src/configuration.c:348 id:3297041220608546238 address:127.0.0.1:4000 role:1
    LIBRAFT   1719786478762868548 src/configuration.c:350 === CONFIG END ===
    LIBDQLITE 1719786478762918142 impl_listen:55 impl listen
    LIBRAFT   1719786478762967585 src/convert.c:23 old_state:0 new_state:1
    LIBRAFT   1719786478763015462 src/convert.c:38 clear follower state
    LIBRAFT   1719786478763056044 src/convert.c:23 old_state:1 new_state:2
    LIBRAFT   1719786478763101528 src/convert.c:168 self elect and convert to leader
    LIBRAFT   1719786478763141442 src/convert.c:49 clear candidate state
    LIBRAFT   1719786478763186925 src/convert.c:23 old_state:2 new_state:3
    LIBDQLITE 1719786478764101607 conn__start:290 conn start
    LIBDQLITE 1719786478764185987 gateway__init:18 gateway init
    LIBDQLITE 1719786479299052685 read_message_cb:186 read error -4095
    LIBDQLITE 1719786479299154749 conn__stop:330 conn stop
    LIBDQLITE 1719786479299209819 gateway__close:80 gateway close
    LIBDQLITE 1719786479299477531 conn__start:290 conn start
    LIBDQLITE 1719786479299523190 gateway__init:18 gateway init
    LIBDQLITE 1719786479299865741 gateway__handle:1280 gateway handle
    LIBDQLITE 1719786479299911680 handle_leader:201 handle leader
    LIBDQLITE 1719786479299965583 handle_leader:216 handle leader - dispatch to 3297041220608546238
    LIBDQLITE 1719786479300103672 gateway__resume:1327 gateway resume - finished
    dqlite-demo: src/lib/transport.c:15: alloc_cb: Assertion `t->read.len > 0' failed.
    ```

### UAF
    * dqlite_test/bin/error_cases/9843/log_app_err0_0
    * https://github.com/canonical/dqlite/pull/669