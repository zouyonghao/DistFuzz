1. Memory leak

    ```
    =================================================================
    ==91926==ERROR: LeakSanitizer: detected memory leaks

    Direct leak of 72 byte(s) in 1 object(s) allocated from:
        #0 0x626a8d in operator new(unsigned long) (/home/zyh/braft/example/atomic/atomic_server+0x626a8d)
        #1 0x658f95 in braft::FileBasedSingleMetaStorage::new_instance(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&) const /home/zyh/braft/src/braft/raft_meta.cpp:555:12
        #2 0x69bceb in braft::RaftMetaStorage::create(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&) /home/zyh/braft/src/braft/storage.cpp:134:18
        #3 0x73c479 in braft::NodeImpl::init_meta_storage() /home/zyh/braft/src/braft/node.cpp:287:21
        #4 0x7411bc in braft::NodeImpl::init(braft::NodeOptions const&) /home/zyh/braft/src/braft/node.cpp:514:9
        #5 0x62b87f in example::Atomic::start() (/home/zyh/braft/example/atomic/atomic_server+0x62b87f)
        #6 0x62995f in main (/home/zyh/braft/example/atomic/atomic_server+0x62995f)
        #7 0x7f7893552bf6 in __libc_start_main /build/glibc-S9d2JN/glibc-2.27/csu/../csu/libc-start.c:310

    Direct leak of 56 byte(s) in 1 object(s) allocated from:
        #0 0x626a8d in operator new(unsigned long) (/home/zyh/braft/example/atomic/atomic_server+0x626a8d)
        #1 0xb5eff4 in brpc::Server::StartInternal(in_addr const&, brpc::PortRange const&, brpc::ServerOptions const*) /home/zyh/incubator-brpc/src/brpc/server.cpp:783:22
        #2 0xb67672 in brpc::Server::Start(butil::EndPoint const&, brpc::ServerOptions const*) /home/zyh/incubator-brpc/src/brpc/server.cpp:1062:12
        #3 0xb67672 in brpc::Server::Start(int, brpc::ServerOptions const*) /home/zyh/incubator-brpc/src/brpc/server.cpp:1081:12
        #4 0x6298d5 in main (/home/zyh/braft/example/atomic/atomic_server+0x6298d5)
        #5 0x7f7893552bf6 in __libc_start_main /build/glibc-S9d2JN/glibc-2.27/csu/../csu/libc-start.c:310

    Indirect leak of 18 byte(s) in 1 object(s) allocated from:
        #0 0x626a8d in operator new(unsigned long) (/home/zyh/braft/example/atomic/atomic_server+0x626a8d)
        #1 0x659083 in void std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >::_M_construct<char*>(char*, char*, std::forward_iterator_tag) /usr/bin/../lib/gcc/x86_64-linux-gnu/7.5.0/../../../../include/c++/7.5.0/bits/basic_string.tcc:219:14
        #2 0x659083 in void std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >::_M_construct_aux<char*>(char*, char*, std::__false_type) /usr/bin/../lib/gcc/x86_64-linux-gnu/7.5.0/../../../../include/c++/7.5.0/bits/basic_string.h:236:11
        #3 0x659083 in void std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >::_M_construct<char*>(char*, char*) /usr/bin/../lib/gcc/x86_64-linux-gnu/7.5.0/../../../../include/c++/7.5.0/bits/basic_string.h:255:4
        #4 0x659083 in std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >::basic_string(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&) /usr/bin/../lib/gcc/x86_64-linux-gnu/7.5.0/../../../../include/c++/7.5.0/bits/basic_string.h:440:9
        #5 0x659083 in braft::FileBasedSingleMetaStorage::FileBasedSingleMetaStorage(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&) /home/zyh/braft/src/braft/raft_meta.h:74:30
        #6 0x659083 in braft::FileBasedSingleMetaStorage::new_instance(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&) const /home/zyh/braft/src/braft/raft_meta.cpp:555:16
        #7 0x69bceb in braft::RaftMetaStorage::create(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&) /home/zyh/braft/src/braft/storage.cpp:134:18
        #8 0x73c479 in braft::NodeImpl::init_meta_storage() /home/zyh/braft/src/braft/node.cpp:287:21
        #9 0x7411bc in braft::NodeImpl::init(braft::NodeOptions const&) /home/zyh/braft/src/braft/node.cpp:514:9
        #10 0x62b87f in example::Atomic::start() (/home/zyh/braft/example/atomic/atomic_server+0x62b87f)
        #11 0x62995f in main (/home/zyh/braft/example/atomic/atomic_server+0x62995f)
        #12 0x7f7893552bf6 in __libc_start_main /build/glibc-S9d2JN/glibc-2.27/csu/../csu/libc-start.c:310

    SUMMARY: AddressSanitizer: 146 byte(s) leaked in 3 allocation(s).
    ```

    https://github.com/baidu/braft/issues/268

2. fail to rename
    issues
        https://github.com/baidu/braft/issues/272
    related logs
        error_cases/100

3. Met raft error
    issues
        https://github.com/baidu/braft/issues/273
    related logs
        error_cases/1486

4. grpc Too many open files

    related logs
        error_cases/1486
    https://github.com/grpc/grpc/issues/20418