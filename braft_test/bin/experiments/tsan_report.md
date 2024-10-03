
pc 0x0000005f2ef5

    ```
    ==================
    ==79754==ERROR: ThreadSanitizer: SEGV on unknown address 0x000000000000 (pc 0x0000005f2ef5 bp 0x000000000000 sp 0x7f421f9fefc8 T79791)
    ==79754==The signal is caused by a WRITE memory access.
    ==79754==Hint: address points to the zero page.
        #0 __sanitizer::internal_memcpy(void*, void const*, unsigned long) <null> (atomic_server+0x5f2ef4)
        #1 __tsan::VarSizeStackTrace::Init(unsigned long const*, unsigned long, unsigned long) <null> (atomic_server+0x5e9407)
        #2 __tsan::ReportRace(__tsan::ThreadState*) <null> (atomic_server+0x5e7126)
        #3 <null> <null> (atomic_server+0x5ec629)
        #4 __tsan_read8 <null> (atomic_server+0x5dbb81)
        #5 braft::operator<(braft::LogId const&, braft::LogId const&) /home/zyh/braft/src/braft/log_entry.h:65:25 (atomic_server+0x686777)
        #6 braft::LogManager::set_disk_id(braft::LogId const&) /home/zyh/braft/src/braft/log_manager.cpp:803:17 (atomic_server+0x686777)
        #7 braft::LogManager::disk_thread(void*, bthread::TaskIterator<braft::LogManager::StableClosure*>&) /home/zyh/braft/src/braft/log_manager.cpp:618:18 (atomic_server+0x686777)
        #8 bthread::ExecutionQueue<braft::LogManager::StableClosure*>::execute_task(void*, void*, bthread::TaskIteratorBase&) /usr/local/include/bthread/execution_queue_inl.h:271:16 (atomic_server+0x690be4)
        #9 bthread::ExecutionQueueBase::_execute(bthread::TaskNode*, bool, int*) /home/zyh/incubator-brpc/src/bthread/execution_queue.cpp:272 (atomic_server+0x7505fc)
        #10 bthread::ExecutionQueueBase::_execute_tasks(void*) /home/zyh/incubator-brpc/src/bthread/execution_queue.cpp:151 (atomic_server+0x7535af)
        #11 bthread::TaskGroup::task_runner(long) /home/zyh/incubator-brpc/src/bthread/task_group.cpp:295 (atomic_server+0x767b6e)
        #12 bthread_make_fcontext <null> (atomic_server+0x8fd990)

    ThreadSanitizer can not provide additional info.
    SUMMARY: ThreadSanitizer: SEGV (/home/zyh/braft/example/atomic/atomic_server+0x5f2ef4) in __sanitizer::internal_memcpy(void*, void const*, unsigned long)
    ==79754==ABORTING
    ==================
    ```

pc 0x0000005f2f60

    ```
    ==================
    ==83678==ERROR: ThreadSanitizer: SEGV on unknown address 0x60000aaaf800 (pc 0x0000005f2f60 bp 0xfffffffffffffeff sp 0x7f6f8acf8648 T83706)
    ==83678==The signal is caused by a READ memory access.
        #0 __sanitizer::internal_memcpy(void*, void const*, unsigned long) <null> (atomic_server+0x5f2f5f)
        #1 __tsan::VarSizeStackTrace::Init(unsigned long const*, unsigned long, unsigned long) <null> (atomic_server+0x5e9407)
        #2 __tsan::ReportRace(__tsan::ThreadState*) <null> (atomic_server+0x5e7126)
        #3 <null> <null> (atomic_server+0x5ec629)
        #4 __tsan_write4 <null> (atomic_server+0x5dcee7)
        #5 braft::Replicator::_block(long, int) /home/zyh/braft/src/braft/replicator.cpp (atomic_server+0x6cacf2)
        #6 braft::Replicator::_on_rpc_returned(unsigned long, brpc::Controller*, braft::AppendEntriesRequest*, braft::AppendEntriesResponse*, long) /home/zyh/braft/src/braft/replicator.cpp:402:19 (atomic_server+0x6ccd16)
        #7 brpc::internal::FunctionClosure5<unsigned long, brpc::Controller*, braft::AppendEntriesRequest*, braft::AppendEntriesResponse*, long>::Run() /usr/local/include/brpc/callback.h:373:5 (atomic_server+0x6d851c)
        #8 brpc::Controller::EndRPC(brpc::Controller::CompletionInfo const&) /home/zyh/incubator-brpc/src/brpc/controller.cpp:912 (atomic_server+0x77cec8)
        #9 brpc::Controller::RunEndRPC(void*) /home/zyh/incubator-brpc/src/brpc/controller.cpp:701 (atomic_server+0x77d13f)
        #10 bthread::TaskGroup::task_runner(long) /home/zyh/incubator-brpc/src/bthread/task_group.cpp:295 (atomic_server+0x767b6e)
        #11 bthread_make_fcontext <null> (atomic_server+0x8fd990)

    ThreadSanitizer can not provide additional info.
    SUMMARY: ThreadSanitizer: SEGV (/home/zyh/braft/example/atomic/atomic_server+0x5f2f5f) in __sanitizer::internal_memcpy(void*, void const*, unsigned long)
    ==83678==ABORTING
    ==================
    ```

pc 0x0000005f2ef5 bp 0x000000000000 sp 0x7fba611ff968

    ```
    ==================
    ==88897==ERROR: ThreadSanitizer: SEGV on unknown address 0x000000000000 (pc 0x0000005f2ef5 bp 0x000000000000 sp 0x7fba611ff968 T88902)
    ==88897==The signal is caused by a WRITE memory access.
    ==88897==Hint: address points to the zero page.
        #0 __sanitizer::internal_memcpy(void*, void const*, unsigned long) <null> (atomic_server+0x5f2ef4)
        #1 __tsan::VarSizeStackTrace::Init(unsigned long const*, unsigned long, unsigned long) <null> (atomic_server+0x5e9407)
        #2 __tsan::ReportRace(__tsan::ThreadState*) <null> (atomic_server+0x5e7126)
        #3 <null> <null> (atomic_server+0x5ec629)
        #4 __tsan_write1 <null> (atomic_server+0x5dc1eb)
        #5 braft::RepeatedTimerTask::on_timedout() /home/zyh/braft/src/braft/repeated_timer_task.cpp:65:15 (atomic_server+0x6c6c8b)
        #6 braft::RepeatedTimerTask::run_on_timedout_in_new_thread(void*) /home/zyh/braft/src/braft/repeated_timer_task.cpp:111:8 (atomic_server+0x6c7179)
        #7 bthread::TaskGroup::task_runner(long) /home/zyh/incubator-brpc/src/bthread/task_group.cpp:295 (atomic_server+0x767b6e)
        #8 bthread_make_fcontext <null> (atomic_server+0x8fd990)

    ThreadSanitizer can not provide additional info.
    SUMMARY: ThreadSanitizer: SEGV (/home/zyh/braft/example/atomic/atomic_server+0x5f2ef4) in __sanitizer::internal_memcpy(void*, void const*, unsigned long)
    ==88897==ABORTING
    ```

pc 0x0000005f2f60 bp 0xfffffffffffffeff sp 0x7f31b96fe698

==================
==89495==ERROR: ThreadSanitizer: SEGV on unknown address 0x6000054bf800 (pc 0x0000005f2f60 bp 0xfffffffffffffeff sp 0x7f31b96fe698 T89516)
==89495==The signal is caused by a READ memory access.
    #0 __sanitizer::internal_memcpy(void*, void const*, unsigned long) <null> (atomic_server+0x5f2f5f)
    #1 __tsan::VarSizeStackTrace::Init(unsigned long const*, unsigned long, unsigned long) <null> (atomic_server+0x5e9407)
    #2 __tsan::ReportRace(__tsan::ThreadState*) <null> (atomic_server+0x5e7126)
    #3 <null> <null> (atomic_server+0x5ec629)
    #4 __tsan_read1 <null> (atomic_server+0x5da4a6)
    #5 braft::ConfigurationChangeDone::Run() /home/zyh/braft/src/braft/node.cpp:90:17 (atomic_server+0x6bb9cb)
    #6 braft::FSMCaller::do_committed(long) /home/zyh/braft/src/braft/fsm_caller.cpp:286:35 (atomic_server+0x6735eb)
    #7 braft::FSMCaller::run(void*, bthread::TaskIterator<braft::FSMCaller::ApplyTask>&) /home/zyh/braft/src/braft/fsm_caller.cpp:126:17 (atomic_server+0x6731b4)
    #8 bthread::ExecutionQueue<braft::FSMCaller::ApplyTask>::execute_task(void*, void*, bthread::TaskIteratorBase&) /usr/local/include/bthread/execution_queue_inl.h:271:16 (atomic_server+0x676b14)
    #9 bthread::ExecutionQueueBase::_execute(bthread::TaskNode*, bool, int*) /home/zyh/incubator-brpc/src/bthread/execution_queue.cpp:272 (atomic_server+0x7505fc)
    #10 bthread::ExecutionQueueBase::_execute_tasks(void*) /home/zyh/incubator-brpc/src/bthread/execution_queue.cpp:151 (atomic_server+0x7535af)
    #11 bthread::TaskGroup::task_runner(long) /home/zyh/incubator-brpc/src/bthread/task_group.cpp:295 (atomic_server+0x767b6e)
    #12 bthread_make_fcontext <null> (atomic_server+0x8fd990)

ThreadSanitizer can not provide additional info.
SUMMARY: ThreadSanitizer: SEGV (/home/zyh/braft/example/atomic/atomic_server+0x5f2f5f) in __sanitizer::internal_memcpy(void*, void const*, unsigned long)
==89495==ABORTING