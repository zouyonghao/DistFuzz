bin_path=/home/zyh/ClickHouse/build/programs

LD_PRELOAD=/home/zyh/DistFuzz/build/preload_module/libdst_preload.so \
__DST_ENV_RANDOM_FILE__=random_node0.txt $bin_path/clickhouse-keeper --config enable_keeper1.xml > run/cmd_log1 2>&1 &
