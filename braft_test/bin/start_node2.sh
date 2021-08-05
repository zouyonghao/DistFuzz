bin_path=/home/zyh/braft/example/atomic

# __DST_ENV_RANDOM_FILE__=random_node2.txt NODE_NAME=node2 "$bin_path"/atomic_server -reuse_addr -reuse_port -port=8303 -conf="127.0.1.1:8304:0,127.0.1.1:8305:0,127.0.1.1:8303:0," -data_path="./data2" >> log2 2>&1 &

# use LD_PRELOAD
LD_PRELOAD=/home/zyh/distributed-system-test/build/test/libtest_preload.so \
__DST_ENV_RANDOM_FILE__=random_node2.txt NODE_NAME=node2 "$bin_path"/atomic_server -reuse_addr -reuse_port -port=8302 -conf="127.0.1.1:8300:0,127.0.1.1:8301:0,127.0.1.1:8302:0," -data_path="./data2" >> log2 2>&1 &
