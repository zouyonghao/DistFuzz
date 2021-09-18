bin_path=/home/zyh/ClickHouse/build/programs

mkdir run

LD_PRELOAD=/home/zyh/distributed-system-test/build/preload_module/libdst_preload.so \
$bin_path/clickhouse-keeper --config enable_keeper1.xml > run/cmd_log1 2>&1 &
LD_PRELOAD=/home/zyh/distributed-system-test/build/preload_module/libdst_preload.so \
$bin_path/clickhouse-keeper --config enable_keeper2.xml > run/cmd_log2 2>&1 &
LD_PRELOAD=/home/zyh/distributed-system-test/build/preload_module/libdst_preload.so \
$bin_path/clickhouse-keeper --config enable_keeper3.xml > run/cmd_log3 2>&1 &
