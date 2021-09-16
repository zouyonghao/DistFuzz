bin_path=/home/zyh/NuRaft/build/examples

LD_PRELOAD=/home/zyh/distributed-system-test/build/preload_module/libdst_preload.so \
__DST_ENV_RANDOM_FILE__=random_node2.txt $bin_path/calc_server 3 127.0.1.1:10003 < calc3.pipe >> calc3.log 2>&1 &