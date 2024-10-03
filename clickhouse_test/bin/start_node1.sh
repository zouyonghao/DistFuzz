bin_path=/home/zyh/ClickHouse/build/programs

LD_PRELOAD=/home/zyh/DistFuzz/build/preload_module/libdst_preload.so \
__DST_ENV_RANDOM_FILE__=random_node1.txt $bin_path/clickhouse-keeper --config enable_keeper2.xml > run/cmd_log2 2>&1 &