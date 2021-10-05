bin_path=/home/zyh/aerospike-server/target/Linux-x86_64/bin

LD_PRELOAD=/home/zyh/distributed-system-test/build/preload_module/libdst_preload.so \
__DST_ENV_RANDOM_FILE__=random_node1.txt $bin_path/asd --config-file aerospike_mesh2.conf > run/log1 2>&1 &