bin_path=/root/floyd/floyd/example/redis/output/bin
PRELOAD_PATH=/root/DistFuzz/build/preload_module/libdst_preload.so

LD_PRELOAD=$PRELOAD_PATH \
__DST_ENV_RANDOM_FILE__=random_node1.txt $bin_path/raftis \
    "127.0.1.1:5001,127.0.1.1:5002,127.0.1.1:5003" \
    "127.0.1.1" 5002 "./data2/" 6479 &