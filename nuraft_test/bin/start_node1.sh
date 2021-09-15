bin_path=/home/zyh/NuRaft/build/examples

LD_PRELOAD=/home/zyh/distributed-system-test/build/preload_module/libdst_preload.so \
__DST_ENV_RANDOM_FILE__=random_node1.txt $bin_path/calc_server 2 127.0.1.1:10002 < calc2.pipe >> calc2.log 2>&1 &