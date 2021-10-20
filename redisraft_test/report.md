1. Assertion `(c->flags & REDIS_SUBSCRIBED || c->flags & REDIS_MONITORING)' failed.
    ```
    20927:C 12 May 2021 02:42:51.216 # oO0OoO0OoO0Oo Redis is starting oO0OoO0OoO0Oo
    20927:C 12 May 2021 02:42:51.217 # Redis version=255.255.255, bits=64, commit=e01c92a5, modified=0, pid=20927, just started
    20927:C 12 May 2021 02:42:51.217 # Configuration loaded
    20927:M 12 May 2021 02:42:51.217 * monotonic clock: POSIX clock_gettime
    20927:M 12 May 2021 02:42:51.221 * Running mode=standalone, port=5004.
    20927:M 12 May 2021 02:42:51.222 # WARNING: The TCP backlog setting of 511 cannot be enforced because /proc/sys/net/core/somaxconn is set to the lower value of 128.
    20927:M 12 May 2021 02:42:51.222 # Server initialized
    20927:M 12 May 2021 02:42:51.222 # WARNING overcommit_memory is set to 0! Background save may fail under low memory condition. To fix this issue add 'vm.overcommit_memory = 1' to /etc/sysctl.conf and then reboot or run the command 'sysctl vm.overcommit_memory=1' for this to take effect.
    20927:M 12 May 2021 02:42:51.223 * <redisraft> RedisRaft starting, arguments: raft-log-filename raftlog3.db addr localhost:5004
    20927:M 12 May 2021 02:42:51.231 * Module 'redisraft' loaded from /home/zyh/redisraft/redisraft.so
    20927:M 12 May 2021 02:42:51.236 * Ready to accept connections
    20927:M 12 May 2021 02:42:54.473 * <redisraft> Joined Raft cluster, node id: 292122770, dbid: 326aceda00f0c4c834826d57001add43
    redis-server: async.c:478: redisProcessCallbacks: Assertion `(c->flags & REDIS_SUBSCRIBED || c->flags & REDIS_MONITORING)' failed.


    === REDIS BUG REPORT START: Cut & paste starting from here ===
    20927:M 12 May 2021 02:42:54.481 # Redis 255.255.255 crashed by signal: 6, si_code: -6
    20927:M 12 May 2021 02:42:54.481 # Killed by PID: 20927, UID: 1000
    20927:M 12 May 2021 02:42:54.481 # Crashed running the instruction at: 0x7f44ea4fffb7

    ------ STACK TRACE ------
    EIP:
    /lib/x86_64-linux-gnu/libc.so.6(gsignal+0xc7)[0x7f44ea4fffb7]

    Backtrace:
    /home/zyh/redis/src/redis-server *:5004(sigsegvHandler+0x18c)[0x63132c]
    /lib/x86_64-linux-gnu/libpthread.so.0(+0x12980)[0x7f44eaadc980]
    /lib/x86_64-linux-gnu/libc.so.6(gsignal+0xc7)[0x7f44ea4fffb7]
    /lib/x86_64-linux-gnu/libc.so.6(abort+0x141)[0x7f44ea501921]
    /lib/x86_64-linux-gnu/libc.so.6(+0x3048a)[0x7f44ea4f148a]
    /lib/x86_64-linux-gnu/libc.so.6(+0x30502)[0x7f44ea4f1502]
    /home/zyh/redisraft/redisraft.so(redisProcessCallbacks+0x5ba)[0x7f44e6db2e4a]
    /home/zyh/redisraft/redisraft.so(redisAsyncHandleRead+0x2b)[0x7f44e6db2f7b]
    /home/zyh/redisraft/redisraft.so(+0x4160b)[0x7f44e6dac60b]
    /home/zyh/redisraft/redisraft.so(uv__io_poll+0x323)[0x7f44e6dce1b3]
    /home/zyh/redisraft/redisraft.so(uv_run+0x128)[0x7f44e6dc0718]
    /home/zyh/redisraft/redisraft.so(+0x2a0ba)[0x7f44e6d950ba]
    /lib/x86_64-linux-gnu/libpthread.so.0(+0x76db)[0x7f44eaad16db]
    /lib/x86_64-linux-gnu/libc.so.6(clone+0x3f)[0x7f44ea5e271f]

    ------ REGISTERS ------
    20927:M 12 May 2021 02:42:54.481 # 
    RAX:0000000000000000 RBX:0000000000000000
    RCX:00007f44ea4fffb7 RDX:0000000000000000
    RDI:0000000000000002 RSI:00007f44e6d650f0
    RBP:00007f44ea678750 RSP:00007f44e6d650f0
    R8 :0000000000000000 R9 :00007f44e6d650f0
    R10:0000000000000008 R11:0000000000000246
    R12:00007f44e6def7b8 R13:00007f44e6dedd78
    R14:00000000000001de R15:00007f44e6d65590
    RIP:00007f44ea4fffb7 EFL:0000000000000246
    CSGSFS:002b000000000033
    20927:M 12 May 2021 02:42:54.481 # (00007f44e6d650ff) -> 0000000000000000
    20927:M 12 May 2021 02:42:54.481 # (00007f44e6d650fe) -> 0000000000000000
    20927:M 12 May 2021 02:42:54.481 # (00007f44e6d650fd) -> 0000000000000000
    20927:M 12 May 2021 02:42:54.481 # (00007f44e6d650fc) -> 00006120000206ec
    20927:M 12 May 2021 02:42:54.481 # (00007f44e6d650fb) -> 00006120000205c0
    20927:M 12 May 2021 02:42:54.481 # (00007f44e6d650fa) -> 00006120000206ec
    20927:M 12 May 2021 02:42:54.481 # (00007f44e6d650f9) -> 0000612000020643
    20927:M 12 May 2021 02:42:54.481 # (00007f44e6d650f8) -> 00006120000205c0
    20927:M 12 May 2021 02:42:54.481 # (00007f44e6d650f7) -> 00006120000205c0
    20927:M 12 May 2021 02:42:54.481 # (00007f44e6d650f6) -> 0000612000020625
    20927:M 12 May 2021 02:42:54.481 # (00007f44e6d650f5) -> 00006120000205c0
    20927:M 12 May 2021 02:42:54.481 # (00007f44e6d650f4) -> 11697092fbad8000
    20927:M 12 May 2021 02:42:54.481 # (00007f44e6d650f3) -> 00007f44ea54970f
    20927:M 12 May 2021 02:42:54.481 # (00007f44e6d650f2) -> 0000000000000083
    20927:M 12 May 2021 02:42:54.481 # (00007f44e6d650f1) -> 00007f44e6d65590
    20927:M 12 May 2021 02:42:54.481 # (00007f44e6d650f0) -> 0000000000000000

    ------ INFO OUTPUT ------
    # Server
    redis_version:255.255.255
    redis_git_sha1:e01c92a5
    redis_git_dirty:0
    redis_build_id:69bef32f3fc9b2cf
    redis_mode:standalone
    os:Linux 4.15.0-139-generic x86_64
    arch_bits:64
    multiplexing_api:epoll
    atomicvar_api:c11-builtin
    gcc_version:4.2.1
    process_id:20927
    process_supervised:no
    run_id:67d81bfd7696aee898fd03419622cd5f3ba86425
    tcp_port:5004
    server_time_usec:1620787374444212
    uptime_in_seconds:3
    uptime_in_days:0
    hz:10
    configured_hz:10
    lru_clock:10174638
    executable:/home/zyh/redis/src/redis-server
    config_file:
    io_threads_active:0

    # Clients
    connected_clients:0
    cluster_connections:0
    maxclients:10000
    client_recent_max_input_buffer:0
    client_recent_max_output_buffer:0
    blocked_clients:0
    tracking_clients:0
    clients_in_timeout_table:0

    # Memory
    used_memory:886064
    used_memory_human:865.30K
    used_memory_rss:12173312
    used_memory_rss_human:11.61M
    used_memory_peak:983112
    used_memory_peak_human:960.07K
    used_memory_peak_perc:90.13%
    used_memory_overhead:878568
    used_memory_startup:878496
    used_memory_dataset:7496
    used_memory_dataset_perc:99.05%
    allocator_allocated:1288520
    allocator_active:1503232
    allocator_resident:4050944
    total_system_memory:4112228352
    total_system_memory_human:3.83G
    used_memory_lua:37888
    used_memory_lua_human:37.00K
    used_memory_scripts:0
    used_memory_scripts_human:0B
    number_of_cached_scripts:0
    maxmemory:0
    maxmemory_human:0B
    maxmemory_policy:noeviction
    allocator_frag_ratio:1.17
    allocator_frag_bytes:214712
    allocator_rss_ratio:2.69
    allocator_rss_bytes:2547712
    rss_overhead_ratio:3.01
    rss_overhead_bytes:8122368
    mem_fragmentation_ratio:13.84
    mem_fragmentation_bytes:11293704
    mem_not_counted_for_evict:0
    mem_replication_backlog:0
    mem_clients_slaves:0
    mem_clients_normal:0
    mem_aof_buffer:0
    mem_allocator:jemalloc-5.1.0
    active_defrag_running:0
    lazyfree_pending_objects:0
    lazyfreed_objects:0

    # Persistence
    loading:0
    current_cow_size:0
    current_cow_size_age:0
    current_fork_perc:0.00
    current_save_keys_processed:0
    current_save_keys_total:0
    rdb_changes_since_last_save:0
    rdb_bgsave_in_progress:0
    rdb_last_save_time:1620787371
    rdb_last_bgsave_status:ok
    rdb_last_bgsave_time_sec:-1
    rdb_current_bgsave_time_sec:-1
    rdb_last_cow_size:0
    aof_enabled:0
    aof_rewrite_in_progress:0
    aof_rewrite_scheduled:0
    aof_last_rewrite_time_sec:-1
    aof_current_rewrite_time_sec:-1
    aof_last_bgrewrite_status:ok
    aof_last_write_status:ok
    aof_last_cow_size:0
    module_fork_in_progress:0
    module_fork_last_cow_size:0

    # Stats
    total_connections_received:1
    total_commands_processed:3
    instantaneous_ops_per_sec:0
    total_net_input_bytes:54
    total_net_output_bytes:5
    instantaneous_input_kbps:0.03
    instantaneous_output_kbps:0.00
    rejected_connections:0
    sync_full:0
    sync_partial_ok:0
    sync_partial_err:0
    expired_keys:0
    expired_stale_perc:0.00
    expired_time_cap_reached_count:0
    expire_cycle_cpu_milliseconds:0
    evicted_keys:0
    keyspace_hits:0
    keyspace_misses:0
    pubsub_channels:0
    pubsub_patterns:0
    latest_fork_usec:0
    total_forks:0
    migrate_cached_sockets:0
    slave_expires_tracked_keys:0
    active_defrag_hits:0
    active_defrag_misses:0
    active_defrag_key_hits:0
    active_defrag_key_misses:0
    tracking_total_keys:0
    tracking_total_items:0
    tracking_total_prefixes:0
    unexpected_error_replies:0
    total_error_replies:0
    dump_payload_sanitizations:0
    total_reads_processed:2
    total_writes_processed:1
    io_threaded_reads_processed:0
    io_threaded_writes_processed:0

    # Replication
    role:master
    connected_slaves:0
    master_failover_state:no-failover
    master_replid:41f5f98e35674d4464f30b1dba04b6f8ced91de4
    master_replid2:0000000000000000000000000000000000000000
    master_repl_offset:0
    second_repl_offset:-1
    repl_backlog_active:0
    repl_backlog_size:1048576
    repl_backlog_first_byte_offset:0
    repl_backlog_histlen:0

    # CPU
    used_cpu_sys:0.028814
    used_cpu_user:0.007203
    used_cpu_sys_children:0.000000
    used_cpu_user_children:0.000000
    used_cpu_sys_main_thread:0.018343
    used_cpu_user_main_thread:0.004585

    # Modules
    module:name=redisraft,ver=1,api=1,filters=1,usedby=[],using=[],options=[]

    # Commandstats
    cmdstat_raft.cluster:calls=1,usec=68,usec_per_call=68.00,rejected_calls=0,failed_calls=0
    cmdstat_config:calls=2,usec=16,usec_per_call=8.00,rejected_calls=0,failed_calls=0

    # Errorstats

    # Cluster
    cluster_enabled:0

    # Keyspace
    db0:keys=1,expires=0,avg_ttl=0

    ------ CLIENT LIST OUTPUT ------

    ------ MODULES INFO OUTPUT ------

    ------ FAST MEMORY TEST ------
    =================================================================
    ==20927==AddressSanitizer CHECK failed: /build/llvm-toolchain-9-uSl4bC/llvm-toolchain-9-9/projects/compiler-rt/lib/asan/asan_thread.cc:356 "((ptr[0] == kCurrentStackFrameMagic)) != (0)" (0x0, 0x0)
        #0 0x4c72ee  (/home/zyh/redis/src/redis-server+0x4c72ee)
        #1 0x4db8ff  (/home/zyh/redis/src/redis-server+0x4db8ff)
        #2 0x4caa42  (/home/zyh/redis/src/redis-server+0x4caa42)
        #3 0x451047  (/home/zyh/redis/src/redis-server+0x451047)
        #4 0x452b72  (/home/zyh/redis/src/redis-server+0x452b72)
        #5 0x4c4b40  (/home/zyh/redis/src/redis-server+0x4c4b40)
        #6 0x45e07b  (/home/zyh/redis/src/redis-server+0x45e07b)
        #7 0x7badf8  (/home/zyh/redis/src/redis-server+0x7badf8)
        #8 0x76cd4a  (/home/zyh/redis/src/redis-server+0x76cd4a)
        #9 0x7da84f  (/home/zyh/redis/src/redis-server+0x7da84f)
        #10 0x7f44eaad0407  (/lib/x86_64-linux-gnu/libpthread.so.0+0x6407)
        #11 0x7f44ea4e2c13  (/lib/x86_64-linux-gnu/libc.so.6+0x21c13)
        #12 0x4474f9  (/home/zyh/redis/src/redis-server+0x4474f9)

    ```

    issues
    * https://github.com/RedisLabs/redisraft/issues/104
    * https://github.com/redis/hiredis/issues/490

2. panic

    ```
    103913:M 17 May 2021 13:17:50.850 # <redisraft> 

    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    REDIS RAFT PANIC
    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    Failed to update log header: raftlog2.db: No such file or directory


    === REDIS BUG REPORT START: Cut & paste starting from here ===
    103913:M 17 May 2021 13:17:50.850 # Redis 255.255.255 crashed by signal: 6, si_code: -6
    103913:M 17 May 2021 13:17:50.850 # Killed by PID: 103913, UID: 1000
    103913:M 17 May 2021 13:17:50.850 # Crashed running the instruction at: 0x7f189b8cafb7

    ------ STACK TRACE ------
    EIP:
    /lib/x86_64-linux-gnu/libc.so.6(gsignal+0xc7)[0x7f189b8cafb7]

    Backtrace:
    /home/zyh/redis/src/redis-server *:5002(sigsegvHandler+0x18c)[0x63132c]
    /lib/x86_64-linux-gnu/libpthread.so.0(+0x12980)[0x7f189bea7980]
    /lib/x86_64-linux-gnu/libc.so.6(gsignal+0xc7)[0x7f189b8cafb7]
    /lib/x86_64-linux-gnu/libc.so.6(abort+0x141)[0x7f189b8cc921]
    /home/zyh/redisraft/redisraft.so(updateLogHeader+0xab)[0x7f189819fe86]
    /home/zyh/redisraft/redisraft.so(RaftLogSetTerm+0x35)[0x7f18981a11af]
    /home/zyh/redisraft/redisraft.so(+0x28a42)[0x7f1898193a42]
    /home/zyh/redisraft/redisraft.so(raft_set_current_term+0x31)[0x7f18981b0571]
    /home/zyh/redisraft/redisraft.so(raft_become_candidate+0x5a)[0x7f18981addba]
    /home/zyh/redisraft/redisraft.so(raft_periodic+0xa5)[0x7f18981b0385]
    /home/zyh/redisraft/redisraft.so(+0x29d6c)[0x7f1898194d6c]
    /home/zyh/redisraft/redisraft.so(uv__run_timers+0x58)[0x7f18981bc0d8]
    /home/zyh/redisraft/redisraft.so(uv_run+0x92)[0x7f18981c0682]
    /home/zyh/redisraft/redisraft.so(+0x2a0ba)[0x7f18981950ba]
    /lib/x86_64-linux-gnu/libpthread.so.0(+0x76db)[0x7f189be9c6db]
    /lib/x86_64-linux-gnu/libc.so.6(clone+0x3f)[0x7f189b9ad71f]

    ------ REGISTERS ------
    103913:M 17 May 2021 13:17:50.850 # 
    RAX:0000000000000000 RBX:00000000006d21e0
    RCX:00007f189b8cafb7 RDX:0000000000000000
    RDI:0000000000000002 RSI:00007f1898168000
    RBP:00007f1898168280 RSP:00007f1898168000
    R8 :0000000000000000 R9 :00007f1898168000
    R10:0000000000000008 R11:0000000000000246
    R12:0000000000000012 R13:0000000000000000
    R14:0000000000000000 R15:0000000000000000
    RIP:00007f189b8cafb7 EFL:0000000000000246
    CSGSFS:002b000000000033
    103913:M 17 May 2021 13:17:50.850 # (00007f189816800f) -> 00007f1898822088
    103913:M 17 May 2021 13:17:50.850 # (00007f189816800e) -> 00007f18981680a0
    103913:M 17 May 2021 13:17:50.850 # (00007f189816800d) -> 00007f1898168100
    103913:M 17 May 2021 13:17:50.850 # (00007f189816800c) -> 00007f18981681c0
    103913:M 17 May 2021 13:17:50.850 # (00007f189816800b) -> 0000000000000000
    103913:M 17 May 2021 13:17:50.850 # (00007f189816800a) -> 00007f189ba418e2
    103913:M 17 May 2021 13:17:50.850 # (00007f1898168009) -> 0000000000000000
    103913:M 17 May 2021 13:17:50.850 # (00007f1898168008) -> 00007f1898168060
    103913:M 17 May 2021 13:17:50.850 # (00007f1898168007) -> 00007f1898167b60
    103913:M 17 May 2021 13:17:50.850 # (00007f1898168006) -> 0000000000000000
    103913:M 17 May 2021 13:17:50.850 # (00007f1898168005) -> 00007f18981680c0
    103913:M 17 May 2021 13:17:50.850 # (00007f1898168004) -> 00007f18981e5a70
    103913:M 17 May 2021 13:17:50.850 # (00007f1898168003) -> 00007f1800000003
    103913:M 17 May 2021 13:17:50.850 # (00007f1898168002) -> 00007f18981682c0
    103913:M 17 May 2021 13:17:50.850 # (00007f1898168001) -> 00000000006d21e0
    103913:M 17 May 2021 13:17:50.850 # (00007f1898168000) -> 0000000000000000

    ------ INFO OUTPUT ------
    # Server
    redis_version:255.255.255
    redis_git_sha1:e01c92a5
    redis_git_dirty:0
    redis_build_id:69bef32f3fc9b2cf
    redis_mode:standalone
    os:Linux 4.15.0-139-generic x86_64
    arch_bits:64
    multiplexing_api:epoll
    atomicvar_api:c11-builtin
    gcc_version:4.2.1
    process_id:103913
    process_supervised:no
    run_id:c66fd3bad5f71029ceb2a190bc0081b576438a4b
    tcp_port:5002
    server_time_usec:1621257470793081
    uptime_in_seconds:48
    uptime_in_days:0
    hz:10
    configured_hz:10
    lru_clock:10644734
    executable:/home/zyh/redis/src/redis-server
    config_file:
    io_threads_active:0

    # Clients
    connected_clients:0
    cluster_connections:0
    maxclients:10000
    client_recent_max_input_buffer:0
    client_recent_max_output_buffer:0
    blocked_clients:0
    tracking_clients:0
    clients_in_timeout_table:0

    # Memory
    used_memory:889000
    used_memory_human:868.16K
    used_memory_rss:16818176
    used_memory_rss_human:16.04M
    used_memory_peak:1095704
    used_memory_peak_human:1.04M
    used_memory_peak_perc:81.14%
    used_memory_overhead:878608
    used_memory_startup:878496
    used_memory_dataset:10392
    used_memory_dataset_perc:98.93%
    allocator_allocated:1592512
    allocator_active:1949696
    allocator_resident:4546560
    total_system_memory:4112228352
    total_system_memory_human:3.83G
    used_memory_lua:37888
    used_memory_lua_human:37.00K
    used_memory_scripts:0
    used_memory_scripts_human:0B
    number_of_cached_scripts:0
    maxmemory:0
    maxmemory_human:0B
    maxmemory_policy:noeviction
    allocator_frag_ratio:1.22
    allocator_frag_bytes:357184
    allocator_rss_ratio:2.33
    allocator_rss_bytes:2596864
    rss_overhead_ratio:3.70
    rss_overhead_bytes:12271616
    mem_fragmentation_ratio:18.96
    mem_fragmentation_bytes:15931032
    mem_not_counted_for_evict:0
    mem_replication_backlog:0
    mem_clients_slaves:0
    mem_clients_normal:0
    mem_aof_buffer:0
    mem_allocator:jemalloc-5.1.0
    active_defrag_running:0
    lazyfree_pending_objects:0
    lazyfreed_objects:0

    # Persistence
    loading:0
    current_cow_size:0
    current_cow_size_age:0
    current_fork_perc:0.00
    current_save_keys_processed:0
    current_save_keys_total:0
    rdb_changes_since_last_save:14
    rdb_bgsave_in_progress:0
    rdb_last_save_time:1621257422
    rdb_last_bgsave_status:ok
    rdb_last_bgsave_time_sec:-1
    rdb_current_bgsave_time_sec:-1
    rdb_last_cow_size:0
    aof_enabled:0
    aof_rewrite_in_progress:0
    aof_rewrite_scheduled:0
    aof_last_rewrite_time_sec:-1
    aof_current_rewrite_time_sec:-1
    aof_last_bgrewrite_status:ok
    aof_last_write_status:ok
    aof_last_cow_size:0
    module_fork_in_progress:0
    module_fork_last_cow_size:0

    # Stats
    total_connections_received:33
    total_commands_processed:170
    instantaneous_ops_per_sec:0
    total_net_input_bytes:11626
    total_net_output_bytes:3465
    instantaneous_input_kbps:0.00
    instantaneous_output_kbps:0.00
    rejected_connections:0
    sync_full:0
    sync_partial_ok:0
    sync_partial_err:0
    expired_keys:0
    expired_stale_perc:0.00
    expired_time_cap_reached_count:0
    expire_cycle_cpu_milliseconds:1
    evicted_keys:0
    keyspace_hits:0
    keyspace_misses:0
    pubsub_channels:0
    pubsub_patterns:0
    latest_fork_usec:0
    total_forks:0
    migrate_cached_sockets:0
    slave_expires_tracked_keys:0
    active_defrag_hits:0
    active_defrag_misses:0
    active_defrag_key_hits:0
    active_defrag_key_misses:0
    tracking_total_keys:0
    tracking_total_items:0
    tracking_total_prefixes:0
    unexpected_error_replies:0
    total_error_replies:31
    dump_payload_sanitizations:0
    total_reads_processed:186
    total_writes_processed:154
    io_threaded_reads_processed:0
    io_threaded_writes_processed:0

    # Replication
    role:master
    connected_slaves:0
    master_failover_state:no-failover
    master_replid:e0765b065ad926072ee131cc8dca8a6b95faf8c8
    master_replid2:0000000000000000000000000000000000000000
    master_repl_offset:0
    second_repl_offset:-1
    repl_backlog_active:0
    repl_backlog_size:1048576
    repl_backlog_first_byte_offset:0
    repl_backlog_histlen:0

    # CPU
    used_cpu_sys:0.209650
    used_cpu_user:0.185386
    used_cpu_sys_children:0.000000
    used_cpu_user_children:0.000000
    used_cpu_sys_main_thread:0.108755
    used_cpu_user_main_thread:0.060419

    # Modules
    module:name=redisraft,ver=1,api=1,filters=1,usedby=[],using=[],options=[]

    # Commandstats
    cmdstat_raft.ae:calls=122,usec=17274,usec_per_call=141.59,rejected_calls=0,failed_calls=0
    cmdstat_raft.cluster:calls=1,usec=36,usec_per_call=36.00,rejected_calls=0,failed_calls=0
    cmdstat_set:calls=14,usec=124,usec_per_call=8.86,rejected_calls=0,failed_calls=0
    cmdstat_raft:calls=31,usec=1826,usec_per_call=58.90,rejected_calls=0,failed_calls=1
    cmdstat_config:calls=2,usec=15,usec_per_call=7.50,rejected_calls=0,failed_calls=0

    # Errorstats
    errorstat_CLUSTERDOWN:count=28
    errorstat_MOVED:count=3

    # Cluster
    cluster_enabled:0

    # Keyspace
    db0:keys=2,expires=0,avg_ttl=0

    ------ CLIENT LIST OUTPUT ------

    ------ MODULES INFO OUTPUT ------

    ------ FAST MEMORY TEST ------
    =================================================================
    ==103913==AddressSanitizer CHECK failed: /build/llvm-toolchain-9-uSl4bC/llvm-toolchain-9-9/projects/compiler-rt/lib/asan/asan_thread.cc:356 "((ptr[0] == kCurrentStackFrameMagic)) != (0)" (0x0, 0x0)
        #0 0x4c72ee  (/home/zyh/redis/src/redis-server+0x4c72ee)
        #1 0x4db8ff  (/home/zyh/redis/src/redis-server+0x4db8ff)
        #2 0x4caa42  (/home/zyh/redis/src/redis-server+0x4caa42)
        #3 0x451047  (/home/zyh/redis/src/redis-server+0x451047)
        #4 0x452b72  (/home/zyh/redis/src/redis-server+0x452b72)
        #5 0x4c4b40  (/home/zyh/redis/src/redis-server+0x4c4b40)
        #6 0x45e07b  (/home/zyh/redis/src/redis-server+0x45e07b)
        #7 0x7badf8  (/home/zyh/redis/src/redis-server+0x7badf8)
        #8 0x76cd4a  (/home/zyh/redis/src/redis-server+0x76cd4a)
        #9 0x7da84f  (/home/zyh/redis/src/redis-server+0x7da84f)
        #10 0x7f189be9b407  (/lib/x86_64-linux-gnu/libpthread.so.0+0x6407)
        #11 0x7f189b8adc13  (/lib/x86_64-linux-gnu/libc.so.6+0x21c13)
        #12 0x4474f9  (/home/zyh/redis/src/redis-server+0x4474f9)

    ```

    issues
    * https://github.com/RedisLabs/redisraft/issues/107

3. heap-buffer-overflow

    ```
    1423:M 25 Sep 2021 14:43:52.986 * <redisraft> RedisRaft starting, arguments: raft-log-filename raftlog2.db addr 127.0.1.1:5002
    1423:M 25 Sep 2021 14:43:52.988 * Module 'redisraft' loaded from /home/zyh/redisraft/redisraft.so
    1423:M 25 Sep 2021 14:43:52.990 * Ready to accept connections
    1423:M 25 Sep 2021 14:44:05.128 # <redisraft> RAFT.NODE ADD failed: connection dropped.
    1423:M 25 Sep 2021 14:44:08.217 # <redisraft> RAFT.NODE ADD failed: NOCLUSTER No Raft Cluster
    1423:M 25 Sep 2021 14:44:08.318 # <redisraft> RAFT.NODE ADD failed: NOCLUSTER No Raft Cluster
    1423:M 25 Sep 2021 14:44:08.418 # <redisraft> RAFT.NODE ADD failed: NOCLUSTER No Raft Cluster
    1423:M 25 Sep 2021 14:44:08.518 # <redisraft> RAFT.NODE ADD failed: NOCLUSTER No Raft Cluster
    1423:M 25 Sep 2021 14:44:08.618 # <redisraft> RAFT.NODE ADD failed: NOCLUSTER No Raft Cluster
    1423:M 25 Sep 2021 14:44:08.718 # <redisraft> RAFT.NODE ADD failed: CLUSTERDOWN No raft leader
    1423:M 25 Sep 2021 14:44:08.818 # <redisraft> RAFT.NODE ADD failed: CLUSTERDOWN No raft leader
    =================================================================
    ==1423==ERROR: AddressSanitizer: heap-buffer-overflow on address 0x6030000100a7 at pc 0x7fdec094185a bp 0x7fdec00fccb0 sp 0x7fdec00fcca8
    READ of size 1 at 0x6030000100a7 thread T1
        #0 0x7fdec0941859  (/home/zyh/redisraft/redisraft.so+0x3e859) node_addr.c:24
        #1 0x7fdec093f77c  (/home/zyh/redisraft/redisraft.so+0x3c77c) common.c:179
        #2 0x7fdec094333b  (/home/zyh/redisraft/redisraft.so+0x4033b) join.c:52
        #3 0x7fdec09ac02c  (/home/zyh/redisraft/redisraft.so+0xa902c) hiredis/async.c:257
        #4 0x7fdec09ac5ab  (/home/zyh/redisraft/redisraft.so+0xa95ab) hiredis/async.c:555
        #5 0x7fdec09a6c06  (/home/zyh/redisraft/redisraft.so+0xa3c06) hiredis/adapters/libuv.h:21
        #6 0x7fdec09c6949  (/home/zyh/redisraft/redisraft.so+0xc3949) deps/libuv/src/unix/linux-core.c:375
        #7 0x7fdec09b9d70  (/home/zyh/redisraft/redisraft.so+0xb6d70) deps/libuv/src/unix/core.c:370
        #8 0x7fdec0959771  (/home/zyh/redisraft/redisraft.so+0x56771) raft.c:952
        #9 0x7fdec523c6da  (/lib/x86_64-linux-gnu/libpthread.so.0+0x76da)
        #10 0x7fdec4d4d71e  (/lib/x86_64-linux-gnu/libc.so.6+0x12171e)

    0x6030000100a7 is located 0 bytes to the right of 23-byte region [0x603000010090,0x6030000100a7)
    allocated by thread T1 here:
        #0 0x4bf3ad  (/home/zyh/redis/src/redis-server+0x4bf3ad)
        #1 0x7fdec09b0de7  (/home/zyh/redisraft/redisraft.so+0xadde7) deps/hiredis/hiredis.c:124

    Thread T1 created by T0 here:
        #0 0x4a9b3a  (/home/zyh/redis/src/redis-server+0x4a9b3a)
        #1 0x7fdec09c4721  (/home/zyh/redisraft/redisraft.so+0xc1721)
        #2 0x7fdec09310e4  (/home/zyh/redisraft/redisraft.so+0x2e0e4)
        #3 0x881c35  (/home/zyh/redis/src/redis-server+0x881c35)
        #4 0x8812ae  (/home/zyh/redis/src/redis-server+0x8812ae)
        #5 0x55ed06  (/home/zyh/redis/src/redis-server+0x55ed06)
        #6 0x7fdec4c4dbf6  (/lib/x86_64-linux-gnu/libc.so.6+0x21bf6)

    SUMMARY: AddressSanitizer: heap-buffer-overflow (/home/zyh/redisraft/redisraft.so+0x3e859)
    Shadow bytes around the buggy address:
    0x0c067fff9fc0: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
    0x0c067fff9fd0: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
    0x0c067fff9fe0: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
    0x0c067fff9ff0: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
    0x0c067fffa000: fd fd fd fa fa fa fd fd fd fd fa fa fd fd fd fd
    =>0x0c067fffa010: fa fa 00 00[07]fa fa fa fa fa fa fa fa fa fa fa
    0x0c067fffa020: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
    0x0c067fffa030: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
        0x0c067fffa040: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
    0x0c067fffa050: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
    0x0c067fffa060: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
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
    ==1423==ABORTING


    === REDIS BUG REPORT START: Cut & paste starting from here ===
    1423:M 25 Sep 2021 14:44:08.923 # Redis 255.255.255 crashed by signal: 6, si_code: -6
    1423:M 25 Sep 2021 14:44:08.923 # Killed by PID: 1423, UID: 1000
    1423:M 25 Sep 2021 14:44:08.923 # Crashed running the instruction at: 0x7fdec4c6afb7

    ------ STACK TRACE ------
    EIP:
    /lib/x86_64-linux-gnu/libc.so.6(gsignal+0xc7)[0x7fdec4c6afb7]

    Backtrace:
    /home/zyh/redis/src/redis-server 127.0.1.1:5002(sigsegvHandler+0x6e8)[0x72d728]
    /lib/x86_64-linux-gnu/libpthread.so.0(+0x12980)[0x7fdec5247980]
    /lib/x86_64-linux-gnu/libc.so.6(gsignal+0xc7)[0x7fdec4c6afb7]
    /lib/x86_64-linux-gnu/libc.so.6(abort+0x141)[0x7fdec4c6c921]
    /home/zyh/redis/src/redis-server 127.0.1.1:5002[0x4dce67]
    /home/zyh/redis/src/redis-server 127.0.1.1:5002[0x4db841]
    /home/zyh/redis/src/redis-server 127.0.1.1:5002[0x4c33c9]
    /home/zyh/redis/src/redis-server 127.0.1.1:5002[0x4c4b3f]
    /home/zyh/redis/src/redis-server 127.0.1.1:5002(__asan_report_load1+0x28)[0x4c5218]
    /home/zyh/redisraft/redisraft.so(NodeAddrParse+0x24a)[0x7fdec094185a]
    /home/zyh/redisraft/redisraft.so(parseMovedReply+0xbd)[0x7fdec093f77d]
    /home/zyh/redisraft/redisraft.so(+0x4033c)[0x7fdec094333c]
    /home/zyh/redisraft/redisraft.so(redisProcessCallbacks+0x39d)[0x7fdec09ac02d]
    /home/zyh/redisraft/redisraft.so(redisAsyncHandleRead+0xbc)[0x7fdec09ac5ac]
    /home/zyh/redisraft/redisraft.so(+0xa3c07)[0x7fdec09a6c07]
    /home/zyh/redisraft/redisraft.so(uv__io_poll+0x30a)[0x7fdec09c694a]
    /home/zyh/redisraft/redisraft.so(uv_run+0x1b1)[0x7fdec09b9d71]
    /home/zyh/redisraft/redisraft.so(+0x56772)[0x7fdec0959772]
    /lib/x86_64-linux-gnu/libpthread.so.0(+0x76db)[0x7fdec523c6db]
    /lib/x86_64-linux-gnu/libc.so.6(clone+0x3f)[0x7fdec4d4d71f]

    ------ REGISTERS ------
    1423:M 25 Sep 2021 14:44:08.923 #
    RAX:0000000000000000 RBX:000000000110b078
    RCX:00007fdec4c6afb7 RDX:0000000000000000
    RDI:0000000000000002 RSI:00007fdec00fbcf0
    RBP:00007fdec00fcc80 RSP:00007fdec00fbcf0
    R8 :0000000000000000 R9 :00007fdec00fbcf0
    R10:0000000000000008 R11:0000000000000246
    R12:00007fdec00fccb0 R13:00007fdec00fcca8
    R14:00007fdec00fcc50 R15:000000000119d4c8
    RIP:00007fdec4c6afb7 EFL:0000000000000246
    CSGSFS:002b000000000033
    1423:M 25 Sep 2021 14:44:08.923 # (00007fdec00fbcff) -> 000000000119d4c8
    1423:M 25 Sep 2021 14:44:08.923 # (00007fdec00fbcfe) -> 00007fdec00fcc50
    1423:M 25 Sep 2021 14:44:08.923 # (00007fdec00fbcfd) -> 00007fdec00fcca8
    1423:M 25 Sep 2021 14:44:08.923 # (00007fdec00fbcfc) -> 00007fdec00fccb0
    1423:M 25 Sep 2021 14:44:08.923 # (00007fdec00fbcfb) -> 000000000110b078
    1423:M 25 Sep 2021 14:44:08.923 # (00007fdec00fbcfa) -> 00007fdec00fbdc0
    1423:M 25 Sep 2021 14:44:08.923 # (00007fdec00fbcf9) -> 00000000004c2bbb
    1423:M 25 Sep 2021 14:44:08.923 # (00007fdec00fbcf8) -> 00007fdec00fbf40
    1423:M 25 Sep 2021 14:44:08.923 # (00007fdec00fbcf7) -> 00007fdec00fc000
    1423:M 25 Sep 2021 14:44:08.923 # (00007fdec00fbcf6) -> 0000003000000008
    1423:M 25 Sep 2021 14:44:08.923 # (00007fdec00fbcf5) -> 00000000004d8760
    1423:M 25 Sep 2021 14:44:08.923 # (00007fdec00fbcf4) -> 00007fdec00fbd80
    1423:M 25 Sep 2021 14:44:08.923 # (00007fdec00fbcf3) -> 00007fdec00fbd80
    1423:M 25 Sep 2021 14:44:08.923 # (00007fdec00fbcf2) -> 00007fdec00fbd80
    1423:M 25 Sep 2021 14:44:08.923 # (00007fdec00fbcf1) -> 00000000004c2bbb
    1423:M 25 Sep 2021 14:44:08.923 # (00007fdec00fbcf0) -> 0000000000000000

    ------ INFO OUTPUT ------
    # Server^M
    redis_version:255.255.255^M
    redis_git_sha1:30df2c4c^M
    redis_git_dirty:0^M
    redis_build_id:edc2bc28dee2a0d^M
    redis_mode:standalone^M
    os:Linux 4.15.0-154-generic x86_64^M
    arch_bits:64^M
    multiplexing_api:epoll^M
    atomicvar_api:c11-builtin^M
    gcc_version:4.2.1^M
    process_id:1423^M
    process_supervised:no^M
    run_id:4a6a263418071a6639cfd0d6bbe8d5d79565fba4^M
    tcp_port:5002^M
    server_time_usec:1632581048847962^M
    uptime_in_seconds:16^M
    uptime_in_days:0^M
    hz:10^M
    configured_hz:10^M
    lru_clock:5191096^M
    executable:/home/zyh/redis/src/redis-server^M
    config_file:^M
    io_threads_active:0^M
    ^M
    # Clients^M
    connected_clients:0^M
    cluster_connections:0^M
    maxclients:10000^M
    client_recent_max_input_buffer:0^M
    client_recent_max_output_buffer:0^M
    blocked_clients:0^M
    tracking_clients:0^M
    clients_in_timeout_table:0^M
    ^M
    # Memory^M
    used_memory:881456^M
    used_memory_human:860.80K^M
    used_memory_rss:32522240^M
    used_memory_rss_human:31.02M^M
    used_memory_peak:983168^M
    used_memory_peak_human:960.12K^M
    used_memory_peak_perc:89.65%^M
    used_memory_overhead:878624^M
    used_memory_startup:878552^M
    used_memory_dataset:2832^M
    used_memory_dataset_perc:97.52%^M
    allocator_allocated:1289688^M
    allocator_active:1523712^M
    allocator_resident:8568832^M
    total_system_memory:67212083200^M
    total_system_memory_human:62.60G^M
    used_memory_lua:37888^M
    used_memory_lua_human:37.00K^M
    used_memory_scripts:0^M
    used_memory_scripts_human:0B^M
    number_of_cached_scripts:0^M
    maxmemory:0^M
    maxmemory_human:0B^M
    maxmemory_policy:noeviction^M
    allocator_frag_ratio:1.18^M
    allocator_frag_bytes:234024^M
    allocator_rss_ratio:5.62^M
    allocator_rss_bytes:7045120^M
    rss_overhead_ratio:3.80^M
    rss_overhead_bytes:23953408^M
    mem_fragmentation_ratio:36.97^M
    mem_fragmentation_bytes:31642576^M
    mem_not_counted_for_evict:0^M
    mem_replication_backlog:0^M
    mem_clients_slaves:0^M
    mem_clients_normal:0^M
    mem_aof_buffer:0^M
    mem_allocator:jemalloc-5.1.0^M
    active_defrag_running:0^M
    lazyfree_pending_objects:0^M
    lazyfreed_objects:0^M
    ^M
    # Persistence^M
    loading:0^M
    current_cow_size:0^M
    current_cow_size_age:0^M
    current_fork_perc:0.00^M
    current_save_keys_processed:0^M
    current_save_keys_total:0^M
    rdb_changes_since_last_save:0^M
    rdb_bgsave_in_progress:0^M
    rdb_last_save_time:1632581032^M
    rdb_last_bgsave_status:ok^M
    rdb_last_bgsave_time_sec:-1^M
    rdb_current_bgsave_time_sec:-1^M
    rdb_last_cow_size:0^M
    aof_enabled:0^M
    aof_rewrite_in_progress:0^M
    aof_rewrite_scheduled:0^M
    aof_last_rewrite_time_sec:-1^M
    aof_current_rewrite_time_sec:-1^M
    aof_last_bgrewrite_status:ok^M
    aof_last_write_status:ok^M
    aof_last_cow_size:0^M
    module_fork_in_progress:0^M
    module_fork_last_cow_size:0^M
    ^M
    # Stats^M
    total_connections_received:1^M
    total_commands_processed:3^M
    instantaneous_ops_per_sec:0^M
    total_net_input_bytes:54^M
    total_net_output_bytes:5^M
    instantaneous_input_kbps:0.00^M
    instantaneous_output_kbps:0.00^M
    rejected_connections:0^M
    sync_full:0^M
    sync_partial_ok:0^M
    sync_partial_err:0^M
    expired_keys:0^M
    expired_stale_perc:0.00^M
    expired_time_cap_reached_count:0^M
    expire_cycle_cpu_milliseconds:1^M
    evicted_keys:0^M
    keyspace_hits:0^M
    keyspace_misses:0^M
    pubsub_channels:0^M
    pubsub_patterns:0^M
    latest_fork_usec:0^M
    total_forks:0^M
    migrate_cached_sockets:0^M
    slave_expires_tracked_keys:0^M
    active_defrag_hits:0^M
    active_defrag_misses:0^M
    active_defrag_key_hits:0^M
    active_defrag_key_misses:0^M
    tracking_total_keys:0^M
    tracking_total_items:0^M
    tracking_total_prefixes:0^M
    unexpected_error_replies:0^M
    total_error_replies:0^M
    dump_payload_sanitizations:0^M
    total_reads_processed:2^M
    total_writes_processed:1^M
    io_threaded_reads_processed:0^M
    io_threaded_writes_processed:0^M
    ^M
    # Replication^M
    role:master^M
    connected_slaves:0^M
    master_failover_state:no-failover^M
    master_replid:539791da21df66297b882dfc40ae2cdfc6af092c^M
    master_replid2:0000000000000000000000000000000000000000^M
    master_repl_offset:0^M
    second_repl_offset:-1^M
    repl_backlog_active:0^M
    repl_backlog_size:1048576^M
    repl_backlog_first_byte_offset:0^M
    repl_backlog_histlen:0^M
    ^M
    # CPU^M
    used_cpu_sys:0.034966^M
    used_cpu_user:0.285162^M
    used_cpu_sys_children:0.000000^M
    used_cpu_user_children:0.000000^M
    used_cpu_sys_main_thread:0.006573^M
    used_cpu_user_main_thread:0.019719^M
    ^M
    # Modules^M
    module:name=redisraft,ver=1,api=1,filters=1,usedby=[],using=[],options=[]^M
    ^M
    # Commandstats^M
    cmdstat_raft.cluster:calls=1,usec=498,usec_per_call=498.00,rejected_calls=0,failed_calls=0^M
    cmdstat_config:calls=2,usec=129,usec_per_call=64.50,rejected_calls=0,failed_calls=0^M
    ^M
    # Errorstats^M
    ^M
    # Cluster^M
    cluster_enabled:0^M
    ^M
    # Keyspace^M
    db0:keys=1,expires=0,avg_ttl=0^M

    ------ CLIENT LIST OUTPUT ------

    ------ MODULES INFO OUTPUT ------

    ------ FAST MEMORY TEST ------
    1423:M 25 Sep 2021 14:44:08.927 # main thread terminated
    1423:M 25 Sep 2021 14:44:08.928 # Bio thread for job type #0 terminated
    1423:M 25 Sep 2021 14:44:08.928 # Bio thread for job type #1 terminated
    1423:M 25 Sep 2021 14:44:08.928 # Bio thread for job type #2 terminated

    Fast memory test PASSED, however your memory can still be broken. Please run a memory test for several hours if possible.

    ------ DUMPING CODE AROUND EIP ------
    Symbol: gsignal (base: 0x7fdec4c6aef0)
    Module: /lib/x86_64-linux-gnu/libc.so.6 (base 0x7fdec4c2c000)
    $ xxd -r -p /tmp/dump.hex /tmp/dump.bin
    $ objdump --adjust-vma=0x7fdec4c6aef0 -D -b binary -m i386:x86-64 /tmp/dump.bin
    ------
    1423:M 25 Sep 2021 14:44:08.928 # dump of function (hexdump of 272 bytes):
    4881ec180100004189f841ba08000000660f6f05a8cf17004989e164488b042528000000488984240801000031c00f29842480000000660f76c0488db424800000004c89ca31ffb80e0000000f298424900000000f298424a00000000f298424b00000000f298424c00000000f298424d00000000f298424e00000000f298424f00000000f05b92700000089c80f054889c7b8ba0000000f0589c64489c2b8ea0000000f05483d00f0ffff773b4189c041ba0800000031d24c89cebf02000000b80e0000000f05488b8c24080100006448330c25280000004489c0751f4881c418010000c30f1f00488b1589be3a00f7d841b8ffffffff648902ebb4e81f5b0f00662e0f1f8400000000000f1f440000
    Function at 0x7fdec4d60b10 is __stack_chk_fail

    === REDIS BUG REPORT END. Make sure to include from START to END. ===

        Please report the crash by opening an issue on github:

            http://github.com/redis/redis/issues

    Suspect RAM error? Use redis-server --test-memory to verify it.

    ```

    issues
    * https://github.com/RedisLabs/redisraft/pull/165

