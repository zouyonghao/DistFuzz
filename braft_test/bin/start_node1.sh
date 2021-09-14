bin_path=/home/zyh/braft/example/atomic

LD_PRELOAD=/home/zyh/distributed-system-test/build/preload_module/libdst_preload.so \
__DST_ENV_RANDOM_FILE__=random_node1.txt NODE_NAME=node1 "$bin_path"/atomic_server -reuse_addr -reuse_port -port=8301 -conf="127.0.1.1:8300:0,127.0.1.1:8301:0,127.0.1.1:8302:0," -data_path="./data1" >> log1 2>&1 &
