bin_path=/home/zyh/ClickHouse/build/programs

LD_PRELOAD=/home/zyh/distributed-system-test/build/preload_module/libdst_preload.so \
__DST_ENV_RANDOM_FILE__=random_node2.txt $bin_path/clickhouse-keeper --config enable_keeper3.xml > run/cmd_log3 2>&1 &