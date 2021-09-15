bin_path=/home/zyh/NuRaft/build/examples

LD_PRELOAD=/home/zyh/distributed-system-test/build/preload_module/libdst_preload.so \
__DST_ENV_RANDOM_FILE__=random_node0.txt $bin_path/calc_server 1 127.0.1.1:10001 < calc1.pipe >> calc1.log 2>&1 &