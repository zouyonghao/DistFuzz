bin_path=/home/zyh/ClickHouse/build/programs

mkdir run

LD_PRELOAD=/home/zyh/DistFuzz/build/preload_module/libdst_preload.so \
$bin_path/clickhouse-keeper --config enable_keeper1.xml > run/cmd_log1 2>&1 &
LD_PRELOAD=/home/zyh/DistFuzz/build/preload_module/libdst_preload.so \
$bin_path/clickhouse-keeper --config enable_keeper2.xml > run/cmd_log2 2>&1 &
LD_PRELOAD=/home/zyh/DistFuzz/build/preload_module/libdst_preload.so \
$bin_path/clickhouse-keeper --config enable_keeper3.xml > run/cmd_log3 2>&1 &

for (( ; ; )) do /usr/share/zookeeper/bin/zkCli.sh -server 127.0.1.1:9181,127.0.1.1:9182,127.0.1.1:9183  2>/dev/null create /a null && break; done