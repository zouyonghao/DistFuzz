bin_path=/root/floyd/floyd/example/redis/output/bin
PRELOAD_PATH=/root/DistFuzz/build/preload_module/libdst_preload.so

LD_PRELOAD=$PRELOAD_PATH \
$bin_path/raftis "127.0.1.1:5001,127.0.1.1:5002,127.0.1.1:5003" "127.0.1.1" 5001 "./data1/" 6379 &

LD_PRELOAD=$PRELOAD_PATH \
$bin_path/raftis "127.0.1.1:5001,127.0.1.1:5002,127.0.1.1:5003" "127.0.1.1" 5002 "./data2/" 6479 &

LD_PRELOAD=$PRELOAD_PATH \
$bin_path/raftis "127.0.1.1:5001,127.0.1.1:5002,127.0.1.1:5003" "127.0.1.1" 5003 "./data3/" 6579 &
