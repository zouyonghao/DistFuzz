### stack-use-after-scope

    ```
    Server got SIGINT from pid 0, uid 0; shutting down...
    =================================================================
    ==17041==ERROR: AddressSanitizer: stack-use-after-scope on address 0x7f40ddefe880 at pc 0x000002b2a55f bp 0x7f40ddefe5b0 sp 0x7f40ddefe5a8
    WRITE of size 8 at 0x7f40ddefe880 thread T9
        #0 0x2b2a55e in epoll_event_queue_t::forget_resource(int, linux_event_callback_t*) /home/zyh/rethinkdb/src/arch/runtime/event_queue/epoll.cc:184:5
        #1 0x2ad0357 in linux_event_watcher_t::remask() /home/zyh/rethinkdb/src/arch/io/event_watcher.cc:204:54
        #2 0x2ad0aa7 in linux_event_watcher_t::stop_watching_for_errors() /home/zyh/rethinkdb/src/arch/io/event_watcher.cc:150:9
        #3 0x2ad093c in linux_event_watcher_t::~linux_event_watcher_t() /home/zyh/rethinkdb/src/arch/io/event_watcher.cc:144:5
        #4 0x2d2def6 in linux_event_fd_watcher_t::~linux_event_fd_watcher_t() /home/zyh/rethinkdb/src/containers/archive/socket_stream.cc:223:1
        #5 0x2d2e04f in linux_event_fd_watcher_t::~linux_event_fd_watcher_t() /home/zyh/rethinkdb/src/containers/archive/socket_stream.cc:221:55
        #6 0x2d0abbe in scoped_ptr_t<fd_watcher_t>::reset() /home/zyh/rethinkdb/./src/containers/scoped.hpp:73:9
        #7 0x2d0a8e8 in scoped_ptr_t<fd_watcher_t>::~scoped_ptr_t() /home/zyh/rethinkdb/./src/containers/scoped.hpp:35:9
        #8 0x2d2ee62 in socket_stream_t::~socket_stream_t() /home/zyh/rethinkdb/src/containers/archive/socket_stream.cc:247:1
        #9 0x2ccf2ab in object_buffer_t<socket_stream_t>::reset() /home/zyh/rethinkdb/./src/containers/object_buffer.hpp:81:23
        #10 0x2cce78f in extproc_worker_t::released(bool, signal_t*) /home/zyh/rethinkdb/src/extproc/extproc_worker.cc:142:19
        #11 0x2ce2d94 in extproc_job_t::~extproc_job_t() /home/zyh/rethinkdb/src/extproc/extproc_job.cc:39:37
        #12 0x2d04418 in http_job_t::~http_job_t() /home/zyh/rethinkdb/./src/extproc/http_job.hpp:13:7
        #13 0x2cfdd72 in http_runner_t::http(http_opts_t const&, http_result_t*, signal_t*) /home/zyh/rethinkdb/src/extproc/http_runner.cc:99:1
        #14 0x246a657 in ql::dispatch_http(ql::env_t*, http_opts_t const&, http_runner_t*, http_result_t*, ql::bt_rcheckable_t const*) /home/zyh/rethinkdb/src/rdb_protocol/terms/http.cc:211:17
        #15 0x387ce9e in version_checker_t::do_check(bool, auto_drainer_t::lock_t) /home/zyh/rethinkdb/src/clustering/administration/main/version_check.cc:73:9
        #16 0x3882503 in void std::__invoke_impl<void, void (version_checker_t::*&)(bool, auto_drainer_t::lock_t), version_checker_t*&, bool&, auto_drainer_t::lock_t&>(std::__invoke_memfun_deref, void (version_checker_t::*&)(bool, auto_drainer_t::lock_t), version_checker_t*&, bool&,
    auto_drainer_t::lock_t&) /usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../../include/c++/9/bits/invoke.h:73:14
        #17 0x3882067 in std::__invoke_result<void (version_checker_t::*&)(bool, auto_drainer_t::lock_t), version_checker_t*&, bool&, auto_drainer_t::lock_t&>::type std::__invoke<void (version_checker_t::*&)(bool, auto_drainer_t::lock_t), version_checker_t*&, bool&, auto_drainer_t::l
    ock_t&>(void (version_checker_t::*&)(bool, auto_drainer_t::lock_t), version_checker_t*&, bool&, auto_drainer_t::lock_t&) /usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../../include/c++/9/bits/invoke.h:95:14
        #18 0x3881f4c in void std::_Bind<void (version_checker_t::* (version_checker_t*, bool, auto_drainer_t::lock_t))(bool, auto_drainer_t::lock_t)>::__call<void, 0ul, 1ul, 2ul>(std::tuple<>&&, std::_Index_tuple<0ul, 1ul, 2ul>) /usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../../inc
    lude/c++/9/functional:400:11
        #19 0x3881ccd in void std::_Bind<void (version_checker_t::* (version_checker_t*, bool, auto_drainer_t::lock_t))(bool, auto_drainer_t::lock_t)>::operator()<void>() /usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../../include/c++/9/functional:482:17
        #20 0x3881742 in callable_action_instance_t<std::_Bind<void (version_checker_t::* (version_checker_t*, bool, auto_drainer_t::lock_t))(bool, auto_drainer_t::lock_t)> >::run_action() /home/zyh/rethinkdb/./src/arch/runtime/callable_action.hpp:32:25
        #21 0x2af7cd1 in callable_action_wrapper_t::run() /home/zyh/rethinkdb/src/arch/runtime/runtime_utils.cc:47:14
        #22 0x2afa44b in coro_t::run() /home/zyh/rethinkdb/src/arch/runtime/coroutines.cc:277:30

    0x7f40ddefe880 is located 120960 bytes inside of 131072-byte region [0x7f40ddee1000,0x7f40ddf01000)
    allocated by thread T9 here:
        #0 0x167d967 in posix_memalign (/home/zyh/rethinkdb/build/debug_clang_system/rethinkdb+0x167d967)

    Thread T9 created by T0 here:
        #0 0x166765a in pthread_create (/home/zyh/rethinkdb/build/debug_clang_system/rethinkdb+0x166765a)
        #1 0x2aea4ed in linux_thread_pool_t::run_thread_pool(linux_thread_message_t*) /home/zyh/rethinkdb/src/arch/runtime/thread_pool.cc:227:19
        #2 0x2b12aaa in run_in_thread_pool(std::function<void ()> const&, int) /home/zyh/rethinkdb/src/arch/runtime/runtime.cc:77:17
        #3 0x380bd65 in main_rethinkdb_porcelain(int, char**) /home/zyh/rethinkdb/src/clustering/administration/main/command_line.cc:2408:9
        #4 0x2d0bab8 in main /home/zyh/rethinkdb/src/main.cc:31:16
        #5 0x7f40f29abbf6 in __libc_start_main /build/glibc-S9d2JN/glibc-2.27/csu/../csu/libc-start.c:310

    SUMMARY: AddressSanitizer: stack-use-after-scope /home/zyh/rethinkdb/src/arch/runtime/event_queue/epoll.cc:184:5 in epoll_event_queue_t::forget_resource(int, linux_event_callback_t*)
    Shadow bytes around the buggy address:
    0x0fe89bbd7cc0: f2 f2 f8 f8 f2 f2 f8 f8 f8 f2 f2 f2 f2 f2 f8 f2
    0x0fe89bbd7cd0: f8 f8 f8 f2 f2 f2 f2 f2 f1 f1 f1 f1 00 f2 f2 f2
    0x0fe89bbd7ce0: 00 04 f2 f2 f8 f8 f8 f8 f8 f8 f8 f8 f8 f8 f8 f8
    0x0fe89bbd7cf0: f8 f8 f8 f8 f8 f8 f8 f8 f8 f8 f8 f8 f8 f8 f8 f8
    0x0fe89bbd7d00: f8 f8 f8 f8 f2 f2 f2 f2 f2 f2 f2 f2 00 f2 f2 f2
    =>0x0fe89bbd7d10:[f8]f3 f3 f3 f2 f2 f2 f2 00 00 f2 f2 f8 f2 f2 f2
    0x0fe89bbd7d20: f8 f2 f2 f2 00 00 00 00 00 00 00 00 00 00 00 00
    0x0fe89bbd7d30: 00 00 00 00 f8 f8 f8 f2 f2 f2 f2 f2 f8 f8 f8 f3
    0x0fe89bbd7d40: f3 f3 f3 f3 00 00 00 00 00 00 00 00 00 00 00 00
    0x0fe89bbd7d50: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
    0x0fe89bbd7d60: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
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
    ==17041==ABORTING
    ```

    https://github.com/rethinkdb/rethinkdb/issues/6956

### memory leak

    ```
    =================================================================
    ==17959==ERROR: LeakSanitizer: detected memory leaks

    Direct leak of 10048 byte(s) in 4 object(s) allocated from:
        #0 0x16ac50d in operator new(unsigned long) (/home/zyh/rethinkdb/build/debug_clang_system/rethinkdb+0x16ac50d)
        #1 0x298a81f in get_TLS_rng() /home/zyh/rethinkdb/src/random.cc:51:21
        #2 0x298aa74 in randsize(unsigned long) /home/zyh/rethinkdb/src/random.cc:65:12
        #3 0x3e50d8e in alt::eviction_bag_t::select_oldish(alt::eviction_bag_t*, unsigned long, alt::page_t**) /home/zyh/rethinkdb/src/buffer_cache/eviction_bag.cc:47:45
        #4 0x3f5c47a in alt::evicter_t::evict_if_necessary() /home/zyh/rethinkdb/src/buffer_cache/evicter.cc:200:15
        #5 0x3f5dc53 in alt::evicter_t::add_to_evictable_disk_backed(alt::page_t*) /home/zyh/rethinkdb/src/buffer_cache/evicter.cc:131:5
        #6 0x3f46ebc in alt::page_t::page_t(unsigned long, buf_ptr_t, counted_t<block_token_t> const&, alt::page_cache_t*) /home/zyh/rethinkdb/src/buffer_cache/page.cc:105:27
        #7 0x3e9a1dc in alt::current_page_t::current_page_t(unsigned long, buf_ptr_t, counted_t<block_token_t> const&, alt::page_cache_t*) /home/zyh/rethinkdb/src/buffer_cache/page_cache.cc:771:17
        #8 0x3e80f6e in alt::page_cache_t::add_read_ahead_buf(unsigned long, scoped_alloc_t<ser_buffer_t, &(void* raw_malloc_aligned<512>(unsigned long)), &(raw_free_aligned(void*))>, counted_t<block_token_t> const&) /home/zyh/rethinkdb/src/buffer_cache/page_cache.cc:186:36
        #9 0x3ec96b4 in void std::__invoke_impl<void, void (alt::page_cache_t::* const&)(unsigned long, scoped_alloc_t<ser_buffer_t, &(void* raw_malloc_aligned<512>(unsigned long)), &(raw_free_aligned(void*))>, counted_t<block_token_t> const&), alt::page_cache_t* const&, unsigned long const&, copyable_unique_t<scoped_alloc_t<ser_buffer_t, &(void* raw_malloc_aligned<512>(unsigned long)), &(raw_free_aligned(void*))> > const&, counted_t<block_token_t> const&>(std::__invoke_memfun_deref, void (alt::page_cache_t::* const&)(unsigned long, scoped_alloc_t<ser_buffer_t, &(void* raw_malloc_aligned<512>(unsigned long)), &(raw_free_aligned(void*))>, counted_t<block_token_t> const&), alt::page_cache_t* const&, unsigned long const&, copyable_unique_t<scoped_alloc_t<ser_buffer_t, &(void* raw_malloc_aligned<512>(unsigned long)), &(raw_free_aligned(void*))> > const&, counted_t<block_token_t> const&) /usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../../include/c++/9/bits/invoke.h:73:14
        #10 0x3ec9097 in std::__invoke_result<void (alt::page_cache_t::* const&)(unsigned long, scoped_alloc_t<ser_buffer_t, &(void* raw_malloc_aligned<512>(unsigned long)), &(raw_free_aligned(void*))>, counted_t<block_token_t> const&), alt::page_cache_t* const&, unsigned long const&, copyable_unique_t<scoped_alloc_t<ser_buffer_t, &(void* raw_malloc_aligned<512>(unsigned long)), &(raw_free_aligned(void*))> > const&, counted_t<block_token_t> const&>::type std::__invoke<void (alt::page_cache_t::* const&)(unsigned long, scoped_alloc_t<ser_buffer_t, &(void* raw_malloc_aligned<512>(unsigned long)), &(raw_free_aligned(void*))>, counted_t<block_token_t> const&), alt::page_cache_t* const&, unsigned long const&, copyable_unique_t<scoped_alloc_t<ser_buffer_t, &(void* raw_malloc_aligned<512>(unsigned long)), &(raw_free_aligned(void*))> > const&, counted_t<block_token_t> const&>(void (alt::page_cache_t::* const&)(unsigned long, scoped_alloc_t<ser_buffer_t, &(void* raw_malloc_aligned<512>(unsigned long)), &(raw_free_aligned(void*))>, counted_t<block_token_t> const&), alt::page_cache_t* const&, unsigned long const&, copyable_unique_t<scoped_alloc_t<ser_buffer_t, &(void* raw_malloc_aligned<512>(unsigned long)), &(raw_free_aligned(void*))> > const&, counted_t<block_token_t> const&) /usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../../include/c++/9/bits/invoke.h:95:14
        #11 0x3ec8f54 in void std::_Bind<void (alt::page_cache_t::* (alt::page_cache_t*, unsigned long, copyable_unique_t<scoped_alloc_t<ser_buffer_t, &(void* raw_malloc_aligned<512>(unsigned long)), &(raw_free_aligned(void*))> >, counted_t<block_token_t>))(unsigned long, scoped_alloc_t<ser_buffer_t, &(void* raw_malloc_aligned<512>(unsigned long)), &(raw_free_aligned(void*))>, counted_t<block_token_t> const&)>::__call_c<void, 0ul, 1ul, 2ul, 3ul>(std::tuple<>&&, std::_Index_tuple<0ul, 1ul, 2ul, 3ul>) const /usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../../include/c++/9/functional:410:11
        #12 0x3ec89cd in void std::_Bind<void (alt::page_cache_t::* (alt::page_cache_t*, unsigned long, copyable_unique_t<scoped_alloc_t<ser_buffer_t, &(void* raw_malloc_aligned<512>(unsigned long)), &(raw_free_aligned(void*))> >, counted_t<block_token_t>))(unsigned long, scoped_alloc_t<ser_buffer_t, &(void* raw_malloc_aligned<512>(unsigned long)), &(raw_free_aligned(void*))>, counted_t<block_token_t> const&)>::operator()<void>() const /usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../../include/c++/9/functional:493:17
        #13 0x3ec8875 in thread_doer_t<std::_Bind<void (alt::page_cache_t::* (alt::page_cache_t*, unsigned long, copyable_unique_t<scoped_alloc_t<ser_buffer_t, &(void* raw_malloc_aligned<512>(unsigned long)), &(raw_free_aligned(void*))> >, counted_t<block_token_t>))(unsigned long, scoped_alloc_t<ser_buffer_t, &(void* raw_malloc_aligned<512>(unsigned long)), &(raw_free_aligned(void*))>, counted_t<block_token_t> const&)> >::do_perform_job() /home/zyh/rethinkdb/./src/do_on_thread.hpp:36:9
        #14 0x3ec7ddd in thread_doer_t<std::_Bind<void (alt::page_cache_t::* (alt::page_cache_t*, unsigned long, copyable_unique_t<scoped_alloc_t<ser_buffer_t, &(void* raw_malloc_aligned<512>(unsigned long)), &(raw_free_aligned(void*))> >, counted_t<block_token_t>))(unsigned long, scoped_alloc_t<ser_buffer_t, &(void* raw_malloc_aligned<512>(unsigned long)), &(raw_free_aligned(void*))>, counted_t<block_token_t> const&)> >::on_thread_switch() /home/zyh/rethinkdb/./src/do_on_thread.hpp:53:17
        #15 0x2ae3545 in linux_message_hub_t::on_event(int) /home/zyh/rethinkdb/src/arch/runtime/message_hub.cc:170:16
        #16 0x2b27edf in epoll_event_queue_t::run() /home/zyh/rethinkdb/src/arch/runtime/event_queue/epoll.cc:115:21
        #17 0x2ae90e5 in linux_thread_pool_t::start_thread(void*) /home/zyh/rethinkdb/src/arch/runtime/thread_pool.cc:185:28
        #18 0x7f3a7b4a06da in start_thread (/lib/x86_64-linux-gnu/libpthread.so.0+0x76da)
    ```

    https://github.com/rethinkdb/rethinkdb/issues/6956

    CVE-2024-22937

### stack-use-after-scope

    ```
    =================================================================
    ==81600==ERROR: AddressSanitizer: stack-use-after-scope on address 0x7f7b35f72f20 at pc 0x000004005b00 bp 0x7f7b35f72ed0 sp 0x7f7b35f72ec8
    WRITE of size 8 at 0x7f7b35f72f20 thread T9
        #0 0x4005aff in directory_map_read_manager_t<server_id_t, empty_value_t>::do_update(peer_id_t, auto_drainer_t::lock_t, auto_drainer_t::lock_t, unsigned long, server_id_t const&, optional<empty_value_t> const&) /home/zyh/rethinkdb/./src/rpc/directory/map_read_manager.tcc
        #1 0x4064315 in void std::__invoke_impl<void, void (directory_map_read_manager_t<server_id_t, empty_value_t>::*&)(peer_id_t, auto_drainer_t::lock_t, auto_drainer_t::lock_t, unsigned long, server_id_t const&, optional<empty_value_t> const&), directory_map_read_manager_t<server_id_t, empty_value_t>*&, peer_id_t&, auto_drainer_t::lock_t&, auto_drainer_t::lock_t&, unsigned long&, server_id_t&, optional<empty_value_t>&>(std::__invoke_memfun_deref, void (directory_map_read_manager_t<server_id_t, empty_value_t>::*&)(peer_id_t, auto_drainer_t::lock_t, auto_drainer_t::lock_t, unsigned long, server_id_t const&, optional<empty_value_t> const&), directory_map_read_manager_t<server_id_t, empty_value_t>*&, peer_id_t&, auto_drainer_t::lock_t&, auto_drainer_t::lock_t&, unsigned long&, server_id_t&, optional<empty_value_t>&) /usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../../include/c++/9/bits/invoke.h:73:14
        #2 0x40639f2 in std::__invoke_result<void (directory_map_read_manager_t<server_id_t, empty_value_t>::*&)(peer_id_t, auto_drainer_t::lock_t, auto_drainer_t::lock_t, unsigned long, server_id_t const&, optional<empty_value_t> const&), directory_map_read_manager_t<server_id_t, empty_value_t>*&, peer_id_t&, auto_drainer_t::lock_t&, auto_drainer_t::lock_t&, unsigned long&, server_id_t&, optional<empty_value_t>&>::type std::__invoke<void (directory_map_read_manager_t<server_id_t, empty_value_t>::*&)(peer_id_t, auto_drainer_t::lock_t, auto_drainer_t::lock_t, unsigned long, server_id_t const&, optional<empty_value_t> const&), directory_map_read_manager_t<server_id_t, empty_value_t>*&, peer_id_t&, auto_drainer_t::lock_t&, auto_drainer_t::lock_t&, unsigned long&, server_id_t&, optional<empty_value_t>&>(void (directory_map_read_manager_t<server_id_t, empty_value_t>::*&)(peer_id_t, auto_drainer_t::lock_t, auto_drainer_t::lock_t, unsigned long, server_id_t const&, optional<empty_value_t> const&), directory_map_read_manager_t<server_id_t, empty_value_t>*&, peer_id_t&, auto_drainer_t::lock_t&, auto_drainer_t::lock_t&, unsigned long&, server_id_t&, optional<empty_value_t>&) /usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../../include/c++/9/bits/invoke.h:95:14
        #3 0x40637f7 in void std::_Bind<void (directory_map_read_manager_t<server_id_t, empty_value_t>::* (directory_map_read_manager_t<server_id_t, empty_value_t>*, peer_id_t, auto_drainer_t::lock_t, auto_drainer_t::lock_t, unsigned long, server_id_t, optional<empty_value_t>))(peer_id_t, auto_drainer_t::lock_t, auto_drainer_t::lock_t, unsigned long, server_id_t const&, optional<empty_value_t> const&)>::__call<void, 0ul, 1ul, 2ul, 3ul, 4ul, 5ul, 6ul>(std::tuple<>&&, std::_Index_tuple<0ul, 1ul, 2ul, 3ul, 4ul, 5ul, 6ul>) /usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../../include/c++/9/functional:400:11
        #4 0x40633ed in void std::_Bind<void (directory_map_read_manager_t<server_id_t, empty_value_t>::* (directory_map_read_manager_t<server_id_t, empty_value_t>*, peer_id_t, auto_drainer_t::lock_t, auto_drainer_t::lock_t, unsigned long, server_id_t, optional<empty_value_t>))(peer_id_t, auto_drainer_t::lock_t, auto_drainer_t::lock_t, unsigned long, server_id_t const&, optional<empty_value_t> const&)>::operator()<void>() /usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../../include/c++/9/functional:482:17
        #5 0x4062632 in callable_action_instance_t<std::_Bind<void (directory_map_read_manager_t<server_id_t, empty_value_t>::* (directory_map_read_manager_t<server_id_t, empty_value_t>*, peer_id_t, auto_drainer_t::lock_t, auto_drainer_t::lock_t, unsigned long, server_id_t, optional<empty_value_t>))(peer_id_t, auto_drainer_t::lock_t, auto_drainer_t::lock_t, unsigned long, server_id_t const&, optional<empty_value_t> const&)> >::run_action() /home/zyh/rethinkdb/./src/arch/runtime/callable_action.hpp:32:25
        #6 0x2a93c38 in callable_action_wrapper_t::run() /home/zyh/rethinkdb/src/arch/runtime/runtime_utils.cc:47:14
        #7 0x2a94f33 in coro_t::run() /home/zyh/rethinkdb/src/arch/runtime/coroutines.cc:277:30

    0x7f7b35f72f20 is located 126752 bytes inside of 131072-byte region [0x7f7b35f54000,0x7f7b35f74000)
    allocated by thread T9 here:
        #0 0x1660b97 in posix_memalign (/home/zyh/rethinkdb/build/release_clang_system/rethinkdb+0x1660b97)

    Thread T9 created by T0 here:
        #0 0x164a88a in pthread_create (/home/zyh/rethinkdb/build/release_clang_system/rethinkdb+0x164a88a)
        #1 0x2a8e125 in linux_thread_pool_t::run_thread_pool(linux_thread_message_t*) /home/zyh/rethinkdb/src/arch/runtime/thread_pool.cc:227:19
        #2 0x2a9dbd8 in run_in_thread_pool(std::function<void ()> const&, int) /home/zyh/rethinkdb/src/arch/runtime/runtime.cc:77:17
        #3 0x372b605 in main_rethinkdb_porcelain(int, char**) /home/zyh/rethinkdb/src/clustering/administration/main/command_line.cc:2408:9
        #4 0x2c81758 in main /home/zyh/rethinkdb/src/main.cc:31:16
        #5 0x7f7b49ab7bf6 in __libc_start_main /build/glibc-S9d2JN/glibc-2.27/csu/../csu/libc-start.c:310

    SUMMARY: AddressSanitizer: stack-use-after-scope /home/zyh/rethinkdb/./src/rpc/directory/map_read_manager.tcc in directory_map_read_manager_t<server_id_t, empty_value_t>::do_update(peer_id_t, auto_drainer_t::lock_t, auto_drainer_t::lock_t, unsigned long, server_id_t const&, optional<empty_value_t> const&)
    Shadow bytes around the buggy address:
    0x0fefe6be6590: f8 f8 f2 f2 f2 f2 f8 f8 f8 f8 f8 f8 f8 f8 f8 f8
    0x0fefe6be65a0: f8 f8 f8 f8 f8 f8 f8 f8 f8 f8 f8 f8 f8 f8 f8 f8
    0x0fefe6be65b0: f8 f8 f8 f8 f8 f8 f8 f8 f8 f8 f8 f8 f8 f8 f8 f8
    0x0fefe6be65c0: f8 f2 f2 f2 f2 f2 f2 f2 f2 f2 f8 f2 f2 f2 f8 f8
    0x0fefe6be65d0: f8 f8 f2 f2 f2 f2 00 00 f2 f2 f8 f2 f1 f1 f1 f1
    =>0x0fefe6be65e0: 00 00 f2 f2[f8]f2 f2 f2 f8 f2 04 f2 f8 f8 f2 f2
    0x0fefe6be65f0: f8 f8 f8 f8 f8 f8 f8 f8 f2 f2 f2 f2 f8 f8 f8 f8
    0x0fefe6be6600: f8 f8 f2 f2 f2 f2 f8 f8 f2 f2 f8 f8 f8 f8 f2 f2
    0x0fefe6be6610: f2 f2 f8 f8 f8 f8 f8 f2 f2 f2 f2 f2 f8 f8 f8 f8
    0x0fefe6be6620: f8 f2 f2 f2 f2 f2 f8 f8 f8 f8 f8 f8 f8 f8 f8 f8
    0x0fefe6be6630: f8 f8 f8 f8 f8 f8 f8 f8 f8 f8 f8 f8 f8 f8 f8 f8
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
    ==81600==ABORTING
    ```

### a bug of clang

    ```
    Version: rethinkdb 2.3.2-windows-beta-588-g4251c1-dirty (CLANG 9.0.0 (tags/RELEASE_900/final))
    error: Error in thread 8 in src/arch/runtime/thread_pool.cc at line 418:
    error: Callstack overflow in a coroutine
    error: Backtrace:
    error: Fri Apr 23 09:11:36 2021

        1 [0x161f52b]: __interceptor_backtrace+0x5b at ??:?
        2 [0x17150f5]: rethinkdb_backtrace(void**, int) at rethinkdb_backtrace.cc:123
        3 [0x2c8641e]: backtrace_t::backtrace_t() at backtrace.cc:213
        4 [0x2c88223]: lazy_backtrace_formatter_t::lazy_backtrace_formatter_t() at backtrace.cc:297
        5 [0x2c856ff]: format_backtrace[abi:cxx11](bool) at backtrace.cc:208
        6 [0x3e66dfd]: report_fatal_error(char const*, int, char const*, ...) at errors.cc:86
        7 [0x2a8d553]: linux_thread_pool_t::fatal_signal_handler(int, siginfo_t*, void*) at thread_pool.cc:418
        8 [0x7f115899c980]: /lib/x86_64-linux-gnu/libpthread.so.0(+0x12980) [0x7f115899c980] at 0x7f115899c980 (/lib/x86_64-linux-gnu/libpthread.so.0)
        9 [0x165f519]: __asan_memcpy+0x199 at ??:?
        10 [0x1ef28b6]: boost::detail::variant::direct_mover<ql::raw_term_t::json_data_t>::operator()(ql::raw_term_t::json_data_t&) at variant.hpp:697
        11 [0x1ef281f]: bool boost::detail::variant::invoke_visitor<boost::detail::variant::direct_mover<ql::raw_term_t::json_data_t> >::internal_visit<ql::raw_term_t::json_data_t>(ql::raw_term_t::json_data_t&, int) at variant.hpp:1048
        12 [0x1ef2786]: boost::detail::variant::invoke_visitor<boost::detail::variant::direct_mover<ql::raw_term_t::json_data_t> >::result_type boost::detail::variant::visitation_impl_invoke_impl<boost::detail::variant::invoke_visitor<boost::detail::variant::direct_mover<ql::raw_term_t::json_data_t> >, void*, ql::raw_term_t::json_data_t>(int, boost::detail::variant::invoke_visitor<boost::detail::variant::direct_mover<ql::raw_term_t::json_data_t> >&, void*, ql::raw_term_t::json_data_t*, mpl_::bool_<true>) at visitation_impl.hpp:112
        13 [0x1ef25df]: boost::detail::variant::invoke_visitor<boost::detail::variant::direct_mover<ql::raw_term_t::json_data_t> >::result_type boost::detail::variant::visitation_impl_invoke<boost::detail::variant::invoke_visitor<boost::detail::variant::direct_mover<ql::raw_term_t::json_data_t> >, void*, ql::raw_term_t::json_data_t, boost::variant<ql::raw_term_t::json_data_t, counted_t<ql::generated_term_t> >::has_fallback_type_>(int, boost::detail::variant::invoke_visitor<boost::detail::variant::direct_mover<ql::raw_term_t::json_data_t> >&, void*, ql::raw_term_t::json_data_t*, boost::variant<ql::raw_term_t::json_data_t, counted_t<ql::generated_term_t> >::has_fallback_type_, int) at visitation_impl.hpp:154
        14 [0x1ef2006]: boost::detail::variant::invoke_visitor<boost::detail::variant::direct_mover<ql::raw_term_t::json_data_t> >::result_type boost::detail::variant::visitation_impl<mpl_::int_<0>, boost::detail::variant::visitation_impl_step<boost::mpl::l_iter<boost::mpl::l_item<mpl_::long_<2l>, ql::raw_term_t::json_data_t, boost::mpl::l_item<mpl_::long_<1l>, counted_t<ql::generated_term_t>, boost::mpl::l_end> > >, boost::mpl::l_iter<boost::mpl::l_end> >, boost::detail::variant::invoke_visitor<boost::detail::variant::direct_mover<ql::raw_term_t::json_data_t> >, void*, boost::variant<ql::raw_term_t::json_data_t, counted_t<ql::generated_term_t> >::has_fallback_type_>(int, int, boost::detail::variant::invoke_visitor<boost::detail::variant::direct_mover<ql::raw_term_t::json_data_t> >&, void*, mpl_::bool_<false>, boost::variant<ql::raw_term_t::json_data_t, counted_t<ql::generated_term_t> >::has_fallback_type_, mpl_::int_<0>*, boost::detail::variant::visitation_impl_step<boost::mpl::l_iter<boost::mpl::l_item<mpl_::long_<2l>, ql::raw_term_t::json_data_t, boost::mpl::l_item<mpl_::long_<1l>, counted_t<ql::generated_term_t>, boost::mpl::l_end> > >, boost::mpl::l_iter<boost::mpl::l_end> >*) at visitation_impl.hpp:238
        15 [0x1ef1e24]: boost::detail::variant::invoke_visitor<boost::detail::variant::direct_mover<ql::raw_term_t::json_data_t> >::result_type boost::variant<ql::raw_term_t::json_data_t, counted_t<ql::generated_term_t> >::internal_apply_visitor_impl<boost::detail::variant::invoke_visitor<boost::detail::variant::direct_mover<ql::raw_term_t::json_data_t> >, void*>(int, int, boost::detail::variant::invoke_visitor<boost::detail::variant::direct_mover<ql::raw_term_t::json_data_t> >&, void*) at variant.hpp:2392
        16 [0x1ef1db3]: boost::detail::variant::invoke_visitor<boost::detail::variant::direct_mover<ql::raw_term_t::json_data_t> >::result_type boost::variant<ql::raw_term_t::json_data_t, counted_t<ql::generated_term_t> >::internal_apply_visitor<boost::detail::variant::invoke_visitor<boost::detail::variant::direct_mover<ql::raw_term_t::json_data_t> > >(boost::detail::variant::invoke_visitor<boost::detail::variant::direct_mover<ql::raw_term_t::json_data_t> >&) at variant.hpp:2406
        17 [0x1ef1b11]: boost::detail::variant::direct_mover<ql::raw_term_t::json_data_t>::result_type boost::variant<ql::raw_term_t::json_data_t, counted_t<ql::generated_term_t> >::apply_visitor<boost::detail::variant::direct_mover<ql::raw_term_t::json_data_t> >(boost::detail::variant::direct_mover<ql::raw_term_t::json_data_t>&) at variant.hpp:2431
        18 [0x1ef1742]: void boost::variant<ql::raw_term_t::json_data_t, counted_t<ql::generated_term_t> >::move_assign<ql::raw_term_t::json_data_t>(ql::raw_term_t::json_data_t&&) at variant.hpp:2170
        19 [0x1ee9299]: boost::enable_if<boost::mpl::and_<boost::is_rvalue_reference<ql::raw_term_t::json_data_t&&>, boost::mpl::not_<boost::is_const<ql::raw_term_t::json_data_t> >, boost::detail::variant::is_variant_constructible_from<ql::raw_term_t::json_data_t&&, boost::mpl::l_item<mpl_::long_<2l>, ql::raw_term_t::json_data_t, boost::mpl::l_item<mpl_::long_<1l>, counted_t<ql::generated_term_t>, boost::mpl::l_end> > >, mpl_::bool_<true>, mpl_::bool_<true> >, boost::variant<ql::raw_term_t::json_data_t, counted_t<ql::generated_term_t> >&>::type boost::variant<ql::raw_term_t::json_data_t, counted_t<ql::generated_term_t> >::operator=<ql::raw_term_t::json_data_t>(ql::raw_term_t::json_data_t&&) at variant.hpp:2197
        20 [0x1ec3f0d]: ql::raw_term_t::init_json(rapidjson::GenericValue<rapidjson::UTF8<char>, rapidjson::MemoryPoolAllocator<rapidjson::RAllocator> > const*) at term_storage.cc:63
        21 [0x1ed41ac]: /home/zyh/rethinkdb/build/release_clang_system//rethinkdb() [0x1ed41ac] at 0x1ed41ac ()
        22 [0x1ed3f19]: /home/zyh/rethinkdb/build/release_clang_system//rethinkdb() [0x1ed3f19] at 0x1ed3f19 ()
        23 [0x1ed3ecf]: /home/zyh/rethinkdb/build/release_clang_system//rethinkdb() [0x1ed3ecf] at 0x1ed3ecf ()
        24 [0x1ed3e46]: /home/zyh/rethinkdb/build/release_clang_system//rethinkdb() [0x1ed3e46] at 0x1ed3e46 ()
        25 [0x1ed3c9f]: /home/zyh/rethinkdb/build/release_clang_system//rethinkdb() [0x1ed3c9f] at 0x1ed3c9f ()
        26 [0x1ed3786]: /home/zyh/rethinkdb/build/release_clang_system//rethinkdb() [0x1ed3786] at 0x1ed3786 ()
        27 [0x1ed35a4]: /home/zyh/rethinkdb/build/release_clang_system//rethinkdb() [0x1ed35a4] at 0x1ed35a4 ()
        28 [0x1ed3533]: /home/zyh/rethinkdb/build/release_clang_system//rethinkdb() [0x1ed3533] at 0x1ed3533 ()
        29 [0x1ed3361]: /home/zyh/rethinkdb/build/release_clang_system//rethinkdb() [0x1ed3361] at 0x1ed3361 ()
        30 [0x1ed3219]: /home/zyh/rethinkdb/build/release_clang_system//rethinkdb() [0x1ed3219] at 0x1ed3219 ()
        31 [0x1ec5dd2]: /home/zyh/rethinkdb/build/release_clang_system//rethinkdb() [0x1ec5dd2] at 0x1ec5dd2 ()
        32 [0x1ec5ab6]: ql::raw_term_t::arg(unsigned long) const at term_storage.cc:130
        33 [0x2442c98]: ql::var_term_t::var_term_t(ql::compile_env_t*, ql::raw_term_t const&) at var.cc:17
        34 [0x2442049]: counted_t<ql::var_term_t> make_counted<ql::var_term_t, ql::compile_env_t*&, ql::raw_term_t const&>(ql::compile_env_t*&, ql::raw_term_t const&) at counted.hpp:133
        35 [0x2441c6a]: ql::make_var_term(ql::compile_env_t*, ql::raw_term_t const&) at var.cc:83
        36 [0x19d3de0]: ql::compile_on_current_stack(ql::compile_env_t*, ql::raw_term_t const&) at term.cc:32
        37 [0x19ddc0f]: /home/zyh/rethinkdb/build/release_clang_system//rethinkdb() [0x19ddc0f] at 0x19ddc0f ()
        38 [0x19dae8f]: /home/zyh/rethinkdb/build/release_clang_system//rethinkdb() [0x19dae8f] at 0x19dae8f ()
        39 [0x19dab33]: ql::compile_term(ql::compile_env_t*, ql::raw_term_t const&) at term.cc:224
        40 [0x269462a]: ql::op_term_t::op_term_t(ql::compile_env_t*, ql::raw_term_t const&, ql::argspec_t, ql::optargspec_t) at op.cc:184
        41 [0x22cc90e]: ql::grouped_seq_op_term_t::grouped_seq_op_term_t<ql::compile_env_t*, ql::raw_term_t, ql::argspec_t, ql::optargspec_t>(ql::compile_env_t*, ql::raw_term_t, ql::argspec_t, ql::optargspec_t) at op.hpp:219
        42 [0x233111c]: ql::bracket_term_t::bracket_term_t(ql::compile_env_t*, ql::raw_term_t const&) at obj_or_seq.cc:296
        43 [0x232e919]: counted_t<ql::bracket_term_t> make_counted<ql::bracket_term_t, ql::compile_env_t*&, ql::raw_term_t const&>(ql::compile_env_t*&, ql::raw_term_t const&) at counted.hpp:133
        44 [0x232c73a]: ql::make_bracket_term(ql::compile_env_t*, ql::raw_term_t const&) at obj_or_seq.cc:356
        45 [0x19d6002]: ql::compile_on_current_stack(ql::compile_env_t*, ql::raw_term_t const&) at term.cc:99
        46 [0x19ddc0f]: /home/zyh/rethinkdb/build/release_clang_system//rethinkdb() [0x19ddc0f] at 0x19ddc0f ()
        47 [0x19dae8f]: /home/zyh/rethinkdb/build/release_clang_system//rethinkdb() [0x19dae8f] at 0x19dae8f ()
        48 [0x19dab33]: ql::compile_term(ql::compile_env_t*, ql::raw_term_t const&) at term.cc:224
        49 [0x269462a]: ql::op_term_t::op_term_t(ql::compile_env_t*, ql::raw_term_t const&, ql::argspec_t, ql::optargspec_t) at op.cc:184
        50 [0x226b9de]: ql::default_term_t::default_term_t(ql::compile_env_t*, ql::raw_term_t const&) at error.cc:28
        51 [0x226a329]: counted_t<ql::default_term_t> make_counted<ql::default_term_t, ql::compile_env_t*&, ql::raw_term_t const&>(ql::compile_env_t*&, ql::raw_term_t const&) at counted.hpp:133
        52 [0x2269e1a]: ql::make_default_term(ql::compile_env_t*, ql::raw_term_t const&) at error.cc:92
        53 [0x19d822b]: ql::compile_on_current_stack(ql::compile_env_t*, ql::raw_term_t const&) at term.cc:154
        54 [0x19ddc0f]: /home/zyh/rethinkdb/build/release_clang_system//rethinkdb() [0x19ddc0f] at 0x19ddc0f ()
        55 [0x19dae8f]: /home/zyh/rethinkdb/build/release_clang_system//rethinkdb() [0x19dae8f] at 0x19dae8f ()
        56 [0x19dab33]: ql::compile_term(ql::compile_env_t*, ql::raw_term_t const&) at term.cc:224
        57 [0x288d7dd]: ql::func_term_t::func_term_t(ql::compile_env_t*, ql::raw_term_t const&) at func.cc:201
        58 [0x19df439]: counted_t<ql::func_term_t> make_counted<ql::func_term_t, ql::compile_env_t*&, ql::raw_term_t const&>(ql::compile_env_t*&, ql::raw_term_t const&) at counted.hpp:133
        59 [0x19d7beb]: ql::compile_on_current_stack(ql::compile_env_t*, ql::raw_term_t const&) at term.cc:144
        60 [0x19ddc0f]: /home/zyh/rethinkdb/build/release_clang_system//rethinkdb() [0x19ddc0f] at 0x19ddc0f ()
        61 [0x19dae8f]: /home/zyh/rethinkdb/build/release_clang_system//rethinkdb() [0x19dae8f] at 0x19dae8f ()
        62 [0x19dab33]: ql::compile_term(ql::compile_env_t*, ql::raw_term_t const&) at term.cc:224
        63 [0x269462a]: ql::op_term_t::op_term_t(ql::compile_env_t*, ql::raw_term_t const&, ql::argspec_t, ql::optargspec_t) at op.cc:184
        64 [0x22ce431]: ql::grouped_seq_op_term_t::grouped_seq_op_term_t<ql::compile_env_t*, ql::raw_term_t, ql::argspec_t>(ql::compile_env_t*, ql::raw_term_t, ql::argspec_t) at op.hpp:219
        65 [0x22db8e4]: ql::concatmap_term_t::concatmap_term_t(ql::compile_env_t*, ql::raw_term_t const&) at seq.cc:353
        66 [0x22b6469]: counted_t<ql::concatmap_term_t> make_counted<ql::concatmap_term_t, ql::compile_env_t*&, ql::raw_term_t const&>(ql::compile_env_t*&, ql::raw_term_t const&) at counted.hpp:133
        67 [0x22b300a]: ql::make_concatmap_term(ql::compile_env_t*, ql::raw_term_t const&) at seq.cc:915
        68 [0x19d5922]: ql::compile_on_current_stack(ql::compile_env_t*, ql::raw_term_t const&) at term.cc:88
        69 [0x19ddc0f]: /home/zyh/rethinkdb/build/release_clang_system//rethinkdb() [0x19ddc0f] at 0x19ddc0f ()
        70 [0x19dae8f]: /home/zyh/rethinkdb/build/release_clang_system//rethinkdb() [0x19dae8f] at 0x19dae8f ()
        71 [0x19dab33]: ql::compile_term(ql::compile_env_t*, ql::raw_term_t const&) at term.cc:224
        72 [0x269462a]: ql::op_term_t::op_term_t(ql::compile_env_t*, ql::raw_term_t const&, ql::argspec_t, ql::optargspec_t) at op.cc:184
        73 [0x22ce431]: ql::grouped_seq_op_term_t::grouped_seq_op_term_t<ql::compile_env_t*, ql::raw_term_t, ql::argspec_t>(ql::compile_env_t*, ql::raw_term_t, ql::argspec_t) at op.hpp:219
        74 [0x22db8e4]: ql::concatmap_term_t::concatmap_term_t(ql::compile_env_t*, ql::raw_term_t const&) at seq.cc:353
        75 [0x22b6469]: counted_t<ql::concatmap_term_t> make_counted<ql::concatmap_term_t, ql::compile_env_t*&, ql::raw_term_t const&>(ql::compile_env_t*&, ql::raw_term_t const&) at counted.hpp:133
        76 [0x22b300a]: ql::make_concatmap_term(ql::compile_env_t*, ql::raw_term_t const&) at seq.cc:915
        77 [0x19d5922]: ql::compile_on_current_stack(ql::compile_env_t*, ql::raw_term_t const&) at term.cc:88
        78 [0x19ddc0f]: /home/zyh/rethinkdb/build/release_clang_system//rethinkdb() [0x19ddc0f] at 0x19ddc0f ()
        79 [0x19dae8f]: /home/zyh/rethinkdb/build/release_clang_system//rethinkdb() [0x19dae8f] at 0x19dae8f ()
        80 [0x19dab33]: ql::compile_term(ql::compile_env_t*, ql::raw_term_t const&) at term.cc:224
        81 [0x269462a]: ql::op_term_t::op_term_t(ql::compile_env_t*, ql::raw_term_t const&, ql::argspec_t, ql::optargspec_t) at op.cc:184
        82 [0x228ad6d]: ql::distinct_term_t::distinct_term_t(ql::compile_env_t*, ql::raw_term_t const&) at sort.cc:137
        83 [0x226f3d9]: counted_t<ql::distinct_term_t> make_counted<ql::distinct_term_t, ql::compile_env_t*&, ql::raw_term_t const&>(ql::compile_env_t*&, ql::raw_term_t const&) at counted.hpp:133
        84 [0x226ea6a]: ql::make_distinct_term(ql::compile_env_t*, ql::raw_term_t const&) at sort.cc:197
        85 [0x19d5b02]: ql::compile_on_current_stack(ql::compile_env_t*, ql::raw_term_t const&) at term.cc:91
        86 [0x19ddc0f]: /home/zyh/rethinkdb/build/release_clang_system//rethinkdb() [0x19ddc0f] at 0x19ddc0f ()
        87 [0x19dae8f]: /home/zyh/rethinkdb/build/release_clang_system//rethinkdb() [0x19dae8f] at 0x19dae8f ()
        88 [0x19dab33]: ql::compile_term(ql::compile_env_t*, ql::raw_term_t const&) at term.cc:224
        89 [0x269462a]: ql::op_term_t::op_term_t(ql::compile_env_t*, ql::raw_term_t const&, ql::argspec_t, ql::optargspec_t) at op.cc:184
        90 [0x2322892]: ql::funcall_term_t::funcall_term_t(ql::compile_env_t*, ql::raw_term_t const&) at control.cc:76
        91 [0x231f519]: counted_t<ql::funcall_term_t> make_counted<ql::funcall_term_t, ql::compile_env_t*&, ql::raw_term_t const&>(ql::compile_env_t*&, ql::raw_term_t const&) at counted.hpp:133
        92 [0x231e94a]: ql::make_funcall_term(ql::compile_env_t*, ql::raw_term_t const&) at control.cc:155
        93 [0x19d7902]: ql::compile_on_current_stack(ql::compile_env_t*, ql::raw_term_t const&) at term.cc:139
        94 [0x19ddc0f]: /home/zyh/rethinkdb/build/release_clang_system//rethinkdb() [0x19ddc0f] at 0x19ddc0f ()
        95 [0x19dae8f]: /home/zyh/rethinkdb/build/release_clang_system//rethinkdb() [0x19dae8f] at 0x19dae8f ()
        96 [0x19dab33]: ql::compile_term(ql::compile_env_t*, ql::raw_term_t const&) at term.cc:224
        97 [0x22a3b99]: ql::make_obj_term_t::make_obj_term_t(ql::compile_env_t*, ql::raw_term_t const&)::{lambda(ql::raw_term_t const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&)#1}::operator()(ql::raw_term_t const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&) const at datum_terms.cc:86
        98 [0x22a356d]: ql::raw_term_t::each_optarg<ql::make_obj_term_t::make_obj_term_t(ql::compile_env_t*, ql::raw_term_t const&)::{lambda(ql::raw_term_t const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&)#1}>(ql::make_obj_term_t::make_obj_term_t(ql::compile_env_t*, ql::raw_term_t const&)::{lambda(ql::raw_term_t const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&)#1}&&) const::{lambda(ql::raw_term_t::json_data_t const&)#1}::operator()(ql::raw_term_t::json_data_t const) const at term_storage.hpp:64
        99 [0x22a2f69]: ql::raw_term_t::source_visitor_t<ql::raw_term_t::each_optarg<ql::make_obj_term_t::make_obj_term_t(ql::compile_env_t*, ql::raw_term_t const&)::{lambda(ql::raw_term_t const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&)#1}>(ql::make_obj_term_t::make_obj_term_t(ql::compile_env_t*, ql::raw_term_t const&)::{lambda(ql::raw_term_t const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&)#1}&&) const::{lambda(ql::raw_term_t::json_data_t const&)#1}, ql::raw_term_t::each_optarg<{lambda(ql::raw_term_t const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&)#1}>({lambda(ql::raw_term_t const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&)#1}) const::{lambda(counted_t<ql::generated_term_t> const&)#1}>::operator()(ql::raw_term_t::json_data_t const) at term_storage.hpp:106
        100 [0x22a2f1f]: void boost::detail::variant::invoke_visitor<ql::raw_term_t::source_visitor_t<ql::raw_term_t::each_optarg<ql::make_obj_term_t::make_obj_term_t(ql::compile_env_t*, ql::raw_term_t const&)::{lambda(ql::raw_term_t const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&)#1}>(ql::make_obj_term_t::make_obj_term_t(ql::compile_env_t*, ql::raw_term_t const&)::{lambda(ql::raw_term_t const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&)#1}&&) const::{lambda(ql::raw_term_t::json_data_t const&)#1}, ql::raw_term_t::each_optarg<{lambda(ql::raw_term_t const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&)#1}>({lambda(ql::raw_term_t const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&)#1}) const::{lambda(counted_t<ql::generated_term_t> const&)#1}> >::internal_visit<ql::raw_term_t::json_data_t>(ql::make_obj_term_t::make_obj_term_t(ql::compile_env_t*, ql::raw_term_t const&)::{lambda(ql::raw_term_t const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&)#1}&, int) at variant.hpp:1048
    error: Exiting.
    ```

### multiple tables
    ```
    com.rethinkdb.gen.exc.ReqlOpFailedError: Table `test.test` is ambiguous; there are multiple tables with that name.
        at com.rethinkdb.ErrorBuilder.build(ErrorBuilder.java:79)
        at com.rethinkdb.net.Response.makeError(Response.java:171)
        at com.rethinkdb.net.Connection.runQuery(Connection.java:288)
        at com.rethinkdb.net.Connection.run(Connection.java:332)
        at com.rethinkdb.net.Connection.run(Connection.java:321)
        at com.rethinkdb.ast.ReqlAst.run(ReqlAst.java:65)
        at cn.edu.tsinghua.oslab.RethinkDBTestClient.cas(RethinkDBTestClient.java:74)
        at cn.edu.tsinghua.oslab.RethinkDBTestClient.main(RethinkDBTestClient.java:105)
    ```

    known issue: https://github.com/rethinkdb/rethinkdb/issues/4521

### Guarantee failed
    ```
    Version: rethinkdb 2.3.2-windows-beta-588-g4251c1-dirty (CLANG 9.0.0 (tags/RELEASE_900/final))
    error: Error in thread 6 in src/rpc/connectivity/cluster.cc at line 1348:
    error: Guarantee failed: [handler != nullptr] Got a message for an unfamiliar tag. Apparently we aren't compatible with the cluster on the other end.
    Listening on driver addresses: 127.0.0.1, ::1
    error: Backtrace:
    Listening on http addresses: 127.0.0.1, ::1
    To fully expose RethinkDB on the network, bind to all addresses by running rethinkdb with the `--bind all` command line option.
    Server ready, "virtual_lt5" e22dca83-aa60-4cf3-af1d-a9aa50d566c8
    Connected to server "virtual_f6w" d66da52a-54f5-494d-bec3-d85ba6b90fdf
    error: Sun Apr 25 08:29:44 2021

        1 [0x7f610b]: __interceptor_backtrace+0x5b at ??:?
        2 [0x126476e]: backtrace_t::backtrace_t() at backtrace.cc:213
        3 [0x1267698]: lazy_backtrace_formatter_t::lazy_backtrace_formatter_t() at backtrace.cc:297
        4 [0x126337a]: format_backtrace[abi:cxx11](bool) at ??:?
        5 [0x1a8f001]: report_fatal_error(char const*, int, char const*, ...) at errors.cc:86
        6 [0x1ac008e]: connectivity_cluster_t::run_t::handle(keepalive_tcp_conn_stream_t*, optional<peer_id_t>, optional<peer_address_t>, optional<server_id_t>, auto_drainer_t::lock_t, bool*, int) at cluster.cc:1346
        7 [0x1ac1890]: connectivity_cluster_t::run_t::connect_to_peer(peer_address_t const*, ip_and_port_t, int, optional<peer_id_t>, optional<server_id_t>, auto_drainer_t::lock_t, bool*, int, co_semaphore_t*) at cluster.cc:360
        8 [0x1ac6e9a]: /home/zyh/rethinkdb/build/release_clang_system/rethinkdb() [0x1ac6e9a] at 0x1ac6e9a ()
        9 [0x1ac2534]: connectivity_cluster_t::run_t::join_blocking(peer_address_t const&, optional<peer_id_t>, optional<server_id_t>, int, auto_drainer_t::lock_t) at pmap.hpp:47
        10 [0x1ad15d7]: callable_action_instance_t<std::_Bind<std::map<ip_and_port_t, join_result_t, std::less<ip_and_port_t>, std::allocator<std::pair<ip_and_port_t const, join_result_t> > > (connectivity_cluster_t::run_t::*(connectivity_cluster_t::run_t*, peer_address_t, r_nullopt_t, r_nullopt_t, int, auto_drainer_t::lock_t))(peer_address_t const&, optional<peer_id_t>, optional<server_id_t>, int, auto_drainer_t::lock_t)> >::run_action() at invoke.h:73
        11 [0x11b27e4]: coro_t::run() at coroutines.cc:277
    error: Exiting.
    ```

    https://github.com/rethinkdb/rethinkdb/issues/3077

### Grarantee failed

    ```
    Version: rethinkdb 2.3.2-windows-beta-588-g4251c1-dirty (CLANG 9.0.0 (tags/RELEASE_900/final))
    error: Error in thread 8 in ./src/containers/map_sentries.hpp at line 70:
    error: Guarantee failed: [iterator_and_is_new.second] value to be inserted already exists. don't do that.
    error: Backtrace:
    error: Sun Apr 25 08:31:58 2021

        1 [0x7f610b]: __interceptor_backtrace+0x5b at ??:?
        2 [0x126476e]: backtrace_t::backtrace_t() at backtrace.cc:213
        3 [0x1267698]: lazy_backtrace_formatter_t::lazy_backtrace_formatter_t() at backtrace.cc:297
        4 [0x126337a]: format_backtrace[abi:cxx11](bool) at ??:?
        5 [0x1a8f001]: report_fatal_error(char const*, int, char const*, ...) at errors.cc:86
        6 [0x1b4190a]: directory_read_manager_t<cluster_directory_metadata_t>::handle_connection(connectivity_cluster_t::connection_t*, auto_drainer_t::lock_t, std::shared_ptr<cluster_directory_metadata_t> const&, fifo_enforcer_state_t, auto_drainer_t::lock_t) at map_sentries.hpp:69
        7 [0x1b4a841]: void std::__invoke_impl<void, void (directory_read_manager_t<cluster_directory_metadata_t>::*&)(connectivity_cluster_t::connection_t*, auto_drainer_t::lock_t, std::shared_ptr<cluster_directory_metadata_t> const&, fifo_enforcer_state_t, auto_drainer_t::lock_t), directory_read_manager_t<cluster_directory_metadata_t>*&, connectivity_cluster_t::connection_t*&, auto_drainer_t::lock_t&, std::shared_ptr<cluster_directory_metadata_t>&, fifo_enforcer_state_t&, auto_drainer_t::lock_t&>(std::__invoke_memfun_deref, void (directory_read_manager_t<cluster_directory_metadata_t>::*&)(connectivity_cluster_t::connection_t*, auto_drainer_t::lock_t, std::shared_ptr<cluster_directory_metadata_t> const&, fifo_enforcer_state_t, auto_drainer_t::lock_t), directory_read_manager_t<cluster_directory_metadata_t>*&, connectivity_cluster_t::connection_t*&, auto_drainer_t::lock_t&, std::shared_ptr<cluster_directory_metadata_t>&, fifo_enforcer_state_t&, auto_drainer_t::lock_t&) at invoke.h:73
        8 [0x1b4a0ee]: callable_action_instance_t<std::_Bind<void (directory_read_manager_t<cluster_directory_metadata_t>::*(directory_read_manager_t<cluster_directory_metadata_t>*, connectivity_cluster_t::connection_t*, auto_drainer_t::lock_t, std::shared_ptr<cluster_directory_metadata_t>, fifo_enforcer_state_t, auto_drainer_t::lock_t))(connectivity_cluster_t::connection_t*, auto_drainer_t::lock_t, std::shared_ptr<cluster_directory_metadata_t> const&, fifo_enforcer_state_t, auto_drainer_t::lock_t)> >::run_action() at callable_action.hpp:32
        9 [0x11b27e4]: coro_t::run() at coroutines.cc:277
    error: Exiting.
    ```

### Guarantee failed: [it->second == nullptr]

    ```
    Version: rethinkdb 2.3.2-windows-beta-588-g4251c1-dirty (CLANG 9.0.0 (tags/RELEASE_900/final))
    error: Error in thread 1 in ./src/clustering/generic/registrar.hpp at line 134:
    error: Guarantee failed: [it->second == nullptr]
    error: Backtrace:
    ==30784==WARNING: ASan is ignoring requested __asan_handle_no_return: stack top: 0x7f63042caec0; bottom 0x7f62fe293000; size: 0x000006037ec0 (100892352)
    False positive error reports may follow
    For details see https://github.com/google/sanitizers/issues/189
    error: Mon Apr 26 04:10:37 2021

        1 [0x7f610b]: __interceptor_backtrace+0x5b at ??:?
        2 [0x126476e]: backtrace_t::backtrace_t() at backtrace.cc:213
        3 [0x1267698]: lazy_backtrace_formatter_t::lazy_backtrace_formatter_t() at backtrace.cc:297
        4 [0x126337a]: format_backtrace[abi:cxx11](bool) at ??:?
        5 [0x1a8f001]: report_fatal_error(char const*, int, char const*, ...) at errors.cc:86
        6 [0x1289b59]: registrar_t<multi_client_client_business_card_t<boost::variant<primary_query_bcard_t::read_request_t, primary_query_bcard_t::write_request_t> >, multi_client_server_t<boost::variant<primary_query_bcard_t::read_request_t, primary_query_bcard_t::write_request_t>, primary_query_server_t*, primary_query_server_t::client_t>*, multi_client_server_t<boost::variant<primary_query_bcard_t::read_request_t, primary_query_bcard_t::write_request_t>, primary_query_server_t*, primary_query_server_t::client_t>::client_t>::on_create(signal_t*, uuid_u, peer_id_t, multi_client_client_business_card_t<boost::variant<primary_query_bcard_t::read_request_t, primary_query_bcard_t::write_request_t> >) at registrar.hpp:134
        7 [0x1294725]: std::_Function_handler<void (signal_t*, uuid_u, peer_id_t, multi_client_client_business_card_t<boost::variant<primary_query_bcard_t::read_request_t, primary_query_bcard_t::write_request_t> >), std::_Bind<void (registrar_t<multi_client_client_business_card_t<boost::variant<primary_query_bcard_t::read_request_t, primary_query_bcard_t::write_request_t> >, multi_client_server_t<boost::variant<primary_query_bcard_t::read_request_t, primary_query_bcard_t::write_request_t>, primary_query_server_t*, primary_query_server_t::client_t>*, multi_client_server_t<boost::variant<primary_query_bcard_t::read_request_t, primary_query_bcard_t::write_request_t>, primary_query_server_t*, primary_query_server_t::client_t>::client_t>::*(registrar_t<multi_client_client_business_card_t<boost::variant<primary_query_bcard_t::read_request_t, primary_query_bcard_t::write_request_t> >, multi_client_server_t<boost::variant<primary_query_bcard_t::read_request_t, primary_query_bcard_t::write_request_t>, primary_query_server_t*, primary_query_server_t::client_t>*, multi_client_server_t<boost::variant<primary_query_bcard_t::read_request_t, primary_query_bcard_t::write_request_t>, primary_query_server_t*, primary_query_server_t::client_t>::client_t>*, std::_Placeholder<1>, std::_Placeholder<2>, std::_Placeholder<3>, std::_Placeholder<4>))(signal_t*, uuid_u, peer_id_t, multi_client_client_business_card_t<boost::variant<primary_query_bcard_t::read_request_t, primary_query_bcard_t::write_request_t> >)> >::_M_invoke(std::_Any_data const&, signal_t*&&, uuid_u&&, peer_id_t&&, multi_client_client_business_card_t<boost::variant<primary_query_bcard_t::read_request_t, primary_query_bcard_t::write_request_t> >&&) at std_function.h:302
        8 [0x1294a1d]: mailbox_t<uuid_u, peer_id_t, multi_client_client_business_card_t<boost::variant<primary_query_bcard_t::read_request_t, primary_query_bcard_t::write_request_t> > >::read_impl_t::read(read_stream_t*, signal_t*) at typed.hpp:51
        9 [0x1afb4ec]: mailbox_manager_t::mailbox_read_coroutine(threadnum_t, unsigned long, std::vector<char, std::allocator<char> >*, long, mailbox_manager_t::force_yield_t) at mailbox.cc:272
        10 [0x11b27e4]: coro_t::run() at coroutines.cc:277
    error: Exiting.
    ```

### Guarantee failed: [peer.ips().size() > 0]

    ```
    Version: rethinkdb 2.3.6-fallback (CLANG 9.0.0 (tags/RELEASE_900/final))
    error: Error in thread 36 in src/rpc/connectivity/cluster.cc at line 395:
    error: Guarantee failed: [peer.ips().size() > 0]
    error: Backtrace:
    error: Tue Jun 29 02:11:10 2021

        1 [0x7f411b]: __interceptor_backtrace+0x5b at ??:?
        2 [0x9ff22e]: backtrace_t::backtrace_t() at backtrace.cc:213
        3 [0xa01e48]: lazy_backtrace_formatter_t::lazy_backtrace_formatter_t() at backtrace.cc:297
        4 [0x9fde3a]: format_backtrace[abi:cxx11](bool) at ??:?
        5 [0x13268f1]: report_fatal_error(char const*, int, char const*, ...) at errors.cc:86
        6 [0x135215b]: connectivity_cluster_t::run_t::join_blocking(peer_address_t const&, optional<peer_id_t>, optional<server_id_t>, int, auto_drainer_t::lock_t) at cluster.cc:395
        7 [0xf68075]: /home/zyh/rethinkdb/build/release_clang_system//rethinkdb() [0xf68075] at 0xf68075 ()
        8 [0x8d2c04]: coro_t::run() at coroutines.cc:277
    error: Exiting.
    ```

### stack overflow
    ```
    ==3712480==WARNING: ASan is ignoring requested __asan_handle_no_return: stack top: 0x7fdd8cdc2c80; bottom 0x7fdd86eac000; size: 0x000005f16c80 (99708032)                                                                                                                               False positive error reports may follow                                                                                                                                                                                                                                                 For details see https://github.com/google/sanitizers/issues/189                                                                                                                                                                                                                         warn: Problem when checking for new versions of RethinkDB: HTTP request to update.rethinkdb.com failed.                                                                                                                                                                                 =================================================================                                                                                                                                                                                                                       ==3712480==ERROR: AddressSanitizer: stack-buffer-overflow on address 0x7fdd86ed10e8 at pc 0x00000082f680 bp 0x7fdd86ed0230 sp 0x7fdd86ecf9f8
    WRITE of size 24 at 0x7fdd86ed10e8 thread T9
        #0 0x82f67f  (/home/zyh/rethinkdb/build/release_clang_system/rethinkdb+0x82f67f)
        #1 0x1a7b81a  (/home/zyh/rethinkdb/build/release_clang_system/rethinkdb+0x1a7b81a)
        #2 0x13cd675  (/home/zyh/rethinkdb/build/release_clang_system/rethinkdb+0x13cd675)
        #3 0x1a756f9  (/home/zyh/rethinkdb/build/release_clang_system/rethinkdb+0x1a756f9)
        #4 0x1a7da81  (/home/zyh/rethinkdb/build/release_clang_system/rethinkdb+0x1a7da81)
        #5 0x939e1e  (/home/zyh/rethinkdb/build/release_clang_system/rethinkdb+0x939e1e)

    0x7fdd86ed10e8 is located 123112 bytes inside of 131072-byte region [0x7fdd86eb3000,0x7fdd86ed3000)
    allocated by thread T9 here:
        #0 0x830c27  (/home/zyh/rethinkdb/build/release_clang_system/rethinkdb+0x830c27)

    Thread T9 created by T0 here:
        #0 0x81a91a  (/home/zyh/rethinkdb/build/release_clang_system/rethinkdb+0x81a91a)
        #1 0x94453f  (/home/zyh/rethinkdb/build/release_clang_system/rethinkdb+0x94453f)
        #2 0x9465c6  (/home/zyh/rethinkdb/build/release_clang_system/rethinkdb+0x9465c6)
        #3 0x179ded4  (/home/zyh/rethinkdb/build/release_clang_system/rethinkdb+0x179ded4)
        #4 0x113445e  (/home/zyh/rethinkdb/build/release_clang_system/rethinkdb+0x113445e)
        #5 0x7fdd90fc20b2  (/lib/x86_64-linux-gnu/libc.so.6+0x270b2)

    SUMMARY: AddressSanitizer: stack-buffer-overflow (/home/zyh/rethinkdb/build/release_clang_system/rethinkdb+0x82f67f)
    Shadow bytes around the buggy address:
    0x0ffc30dd21c0: 00 00 00 00 f1 f1 f1 f1 00 f3 f3 f3 00 00 00 00
    0x0ffc30dd21d0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
    0x0ffc30dd21e0: 00 00 00 00 00 00 00 00 00 00 00 00 f1 f1 f1 f1
    0x0ffc30dd21f0: 00 00 00 f2 f2 f2 f2 f2 00 00 00 00 f2 f2 f2 f2
    0x0ffc30dd2200: 04 f2 00 00 00 00 00 00 00 00 00 00 00 f2 f2 f2
    =>0x0ffc30dd2210: f2 f2 00 00 01 f2 f2 f2 f2 f2 00 00 00[f2]f2 f2
    0x0ffc30dd2220: 04 f2 f8 f8 f8 f8 f8 f8 f8 f8 f8 f8 00 00 00 00
    0x0ffc30dd2230: 00 00 00 00 00 00 00 00 00 00 00 00 f2 f2 f8 f8
    0x0ffc30dd2240: f8 f8 f8 f8 f8 f8 f2 f2 f2 f2 f8 f8 f8 f8 f8 f8
    0x0ffc30dd2250: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
    0x0ffc30dd2260: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
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
    ==3712480==ABORTING
    ```

more bugs:

    cat test_cases/*/log[0-2] | grep 'error: Error' | awk '{print $7 $10}' | sort | uniq
        src/arch/runtime/thread_pool.cc420:
        ./src/clustering/generic/registrar.hpp134:
        ./src/concurrency/watchable_map.tcc99:
        ./src/containers/intrusive_list.hpp175:
        ./src/containers/map_sentries.hpp70:
        src/rpc/connectivity/cluster.cc1348:
        src/rpc/connectivity/cluster.cc395:
        ./src/rpc/directory/read_manager.tcc48:
        ./src/rpc/semilattice/semilattice_manager.tcc516: