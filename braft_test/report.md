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

2. fail to rename *

   issues
    * https://github.com/baidu/braft/issues/272

   related logs
    * error_cases/100

3. Met raft error *

   issues
    * https://github.com/baidu/braft/issues/273

   related logs
    * error_cases/1486

4. grpc Too many open files

   issues
    * https://github.com/grpc/grpc/issues/20418

   related logs
    * error_cases/1486

5. Check failed: meta.term == header.term

    ```
    I0418 23:23:48.741300 83193 /home/zyh/braft/example/atomic/server.cpp:300] Node becomes leader
    F0418 23:23:48.834052 83193 /home/zyh/braft/src/braft/log.cpp:463] Check failed: meta.term == header.term (5 vs 6). 
    #0 0x000000b76fd0 logging::LogMessage::~LogMessage()
    #1 0x0000008dc0e1 braft::Segment::get()
    #2 0x0000008f41af braft::SegmentLogStorage::get_entry()
    #3 0x000000932fd6 braft::LogManager::get_entry()
    #4 0x000000a767b2 braft::Replicator::_prepare_entry()
    #5 0x000000a6fd37 braft::Replicator::_send_entries()
    #6 0x000000a6d69f braft::Replicator::_on_rpc_returned()
    #7 0x000000a9814b brpc::internal::FunctionClosure5<>::Run()
    #8 0x000000bcaab9 brpc::Controller::EndRPC()
    #9 0x000000bcaeeb brpc::Controller::OnVersionedRPCReturned()
    #10 0x000000bf84e5 brpc::policy::ProcessRpcResponse()
    #11 0x000000bef587 brpc::ProcessInputMessage()
    #12 0x000000bf0442 brpc::InputMessenger::OnNewMessages()
    #13 0x000000cb3dfd brpc::Socket::ProcessEvent()
    #14 0x000000bb575f bthread::TaskGroup::task_runner()
    #15 0x000000d4b051 bthread_make_fcontext

    F0418 23:23:48.849531 83193 /home/zyh/braft/src/braft/log.cpp:463] Check failed: meta.term == header.term (5 vs 6). 
    #0 0x000000b76fd0 logging::LogMessage::~LogMessage()
    #1 0x0000008dc0e1 braft::Segment::get()
    #2 0x0000008f41af braft::SegmentLogStorage::get_entry()
    #3 0x000000932fd6 braft::LogManager::get_entry()
    #4 0x000000a767b2 braft::Replicator::_prepare_entry()
    #5 0x000000a6fd37 braft::Replicator::_send_entries()
    #6 0x000000a6d69f braft::Replicator::_on_rpc_returned()
    #7 0x000000a9814b brpc::internal::FunctionClosure5<>::Run()
    #8 0x000000bcaab9 brpc::Controller::EndRPC()
    #9 0x000000bcaeeb brpc::Controller::OnVersionedRPCReturned()
    #10 0x000000bf84e5 brpc::policy::ProcessRpcResponse()
    #11 0x000000bef587 brpc::ProcessInputMessage()
    #12 0x000000bf0442 brpc::InputMessenger::OnNewMessages()
    #13 0x000000cb3dfd brpc::Socket::ProcessEvent()
    #14 0x000000bb575f bthread::TaskGroup::task_runner()
    #15 0x000000d4b051 bthread_make_fcontext

    I0418 23:23:51.457718 83193 /home/zyh/braft/src/braft/node.cpp:2009] node Atomic:127.0.1.1:8301:0 received PreVote from 127.0.1.1:8302:0 in term 7 current_term 7 granted 0

    ```
   issues
    * https://github.com/baidu/braft/issues/279

   related logs
    * error_cases/8181

6. Check failed: false. Did you forget to call truncate_suffix

    ```
    F0420 08:05:14.767930 83223 /home/zyh/braft/src/braft/log_manager.cpp:310] Can't truncate logs before _applied_id=9, last_log_kept=8
    F0420 08:05:14.776836 83223 /home/zyh/braft/src/braft/configuration_manager.cpp:24] Check failed: false. Did you forget to call truncate_suffix before  the last log index goes back
    #0 0x000000b76fd0 logging::LogMessage::~LogMessage()
    #1 0x000000b56111 braft::ConfigurationManager::add()
    #2 0x00000092cd43 braft::LogManager::append_entries()
    #3 0x0000009cc7d4 braft::NodeImpl::handle_append_entries_request()
    #4 0x000000a406e1 braft::RaftServiceImpl::append_entries()
    #5 0x0000008907ca braft::RaftService::CallMethod()
    #6 0x000000bfb833 brpc::policy::ProcessRpcRequest()
    #7 0x000000bef587 brpc::ProcessInputMessage()
    #8 0x000000bf0442 brpc::InputMessenger::OnNewMessages()
    #9 0x000000cb3dfd brpc::Socket::ProcessEvent()
    #10 0x000000bb575f bthread::TaskGroup::task_runner()
    #11 0x000000d4b051 bthread_make_fcontext

    F0420 08:05:14.776913 83218 /home/zyh/braft/src/braft/log.cpp:720] There's gap between appending entries and _last_log_index path: ./data0/log

    ```
   issues
    * https://github.com/baidu/braft/issues/280

   related logs
    * error_cases/8181

7. too many `reject term_unmatched AppendEntries`

    issues
    * https://github.com/baidu/braft/issues/330

8. 一个Node stepped down 之后，集群无法响应客户端请求

    issues
    * https://github.com/baidu/braft/issues/338

9. brpc ASan error
    ```
    E1201 22:37:01.713448 34144 /home/zyh/brpc/src/bthread/task_control.cpp:167] Fail to create _workers[2], Resource temporarily unavailable
    E1201 22:37:01.713607 34144 /home/zyh/brpc/src/bthread/bthread.cpp:92] Fail to init g_task_control
    ASAN:DEADLYSIGNAL
    =================================================================
    ==34144==ERROR: AddressSanitizer: SEGV on unknown address 0x7fc5d05ac9d0 (pc 0x7fc5e7ec4386 bp 0x7ffd1e92b4c0 sp 0x7ffd1e92b460 T0)
    ==34144==The signal is caused by a READ memory access.
    E1201 22:37:01.729068 34147 /home/zyh/brpc/src/bthread/task_control.cpp:68] Fail to create TaskGroup in pthread=140487748658944
    E1201 22:37:01.729776 34148 /home/zyh/brpc/src/bthread/task_control.cpp:68] Fail to create TaskGroup in pthread=140487664776960
        #0 0x7fc5e7ec4385  (/lib/x86_64-linux-gnu/libpthread.so.0+0xf385)
        #1 0x55c92471ddd4  (/home/zyh/braft/example/atomic/atomic_test+0x1a4dd4) brpc/src/bthread/task_control.cpp:240
        #2 0x55c92472088a  (/home/zyh/braft/example/atomic/atomic_test+0x1a788a) brpc/src/bthread/task_control.cpp:257
        #3 0x55c92471cb75  (/home/zyh/braft/example/atomic/atomic_test+0x1a3b75) brpc/src/bthread/bthread.cpp:93
        #4 0x55c924764451  (/home/zyh/braft/example/atomic/atomic_test+0x1eb451) brpc/src/brpc/global.cpp:607
        #5 0x7fc5e7ec4906  (/lib/x86_64-linux-gnu/libpthread.so.0+0xf906)
        #6 0x55c92476130c  (/home/zyh/braft/example/atomic/atomic_test+0x1e830c)
        #7 0x55c92473d280  (/home/zyh/braft/example/atomic/atomic_test+0x1c4280)
        #8 0x55c92473db2e  (/home/zyh/braft/example/atomic/atomic_test+0x1c4b2e)
        #9 0x55c9246d6e45  (/home/zyh/braft/example/atomic/atomic_test+0x15de45)
        #10 0x55c9246c5895  (/home/zyh/braft/example/atomic/atomic_test+0x14c895)
        #11 0x55c9246b5347  (/home/zyh/braft/example/atomic/atomic_test+0x13c347)
        #12 0x7fc5e6f49bf6  (/lib/x86_64-linux-gnu/libc.so.6+0x21bf6)
        #13 0x55c9246c3569  (/home/zyh/braft/example/atomic/atomic_test+0x14a569)

    AddressSanitizer can not provide additional info.
    SUMMARY: AddressSanitizer: SEGV (/lib/x86_64-linux-gnu/libpthread.so.0+0xf385)
    ==34144==ABORTING
    ```

    issues
    * https://github.com/apache/incubator-brpc/issues/1624

10. Fatal error
    ```
    F1201 22:36:50.517096 33233 /home/zyh/brpc/src/brpc/event_dispatcher.cpp:360] Check failed: 0 == g_edisp[i].Start(&attr) (0 vs -1).
    #0 0x55dbcc135d30 logging::LogMessage::~LogMessage()
    #1 0x55dbcc1a1c0c brpc::InitializeGlobalDispatchers()
    #2 0x7f043201c907 __pthread_once_slow
    #3 0x55dbcc1a22be brpc::GetGlobalEventDispatcher()
    #4 0x55dbcc26b7bb brpc::Socket::ResetFileDescriptor()
    #5 0x55dbcc26f8d7 brpc::Socket::Create()
    #6 0x55dbcc336c70 brpc::Acceptor::StartAccept()
    #7 0x55dbcc25e500 brpc::Server::StartInternal()
    #8 0x55dbcc25edff brpc::Server::Start()
    #9 0x55dbcc25efa3 brpc::Server::Start()
    #10 0x55dbcbe8a413 main
    #11 0x7f04310a1bf7 __libc_start_main
    #12 0x55dbcbea5aaa _start
    ```