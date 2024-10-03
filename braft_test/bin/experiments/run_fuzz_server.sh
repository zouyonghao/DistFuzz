bin_path=/home/zyh/braft/example/atomic

./start_event_control.sh
./start_node0.sh
./start_node1.sh
./start_node2.sh
# ./start_proxy_server.sh

# use LD_PRELOAD
# LD_PRELOAD=/home/zyh/DistFuzz/build/test/libtest_preload.so __DST_ENV_RANDOM_FILE__=random_node0.txt NODE_NAME=node0 "$bin_path"/atomic_server -reuse_addr -port=8300 -conf="127.0.1.1:8300:0,127.0.1.1:8301:0,127.0.1.1:8302:0," -data_path="./data0" > log0 2>&1 &
# LD_PRELOAD=/home/zyh/DistFuzz/build/test/libtest_preload.so __DST_ENV_RANDOM_FILE__=random_node1.txt NODE_NAME=node1 "$bin_path"/atomic_server -reuse_addr -port=8301 -conf="127.0.1.1:8300:0,127.0.1.1:8301:0,127.0.1.1:8302:0," -data_path="./data1" > log1 2>&1 &
# LD_PRELOAD=/home/zyh/DistFuzz/build/test/libtest_preload.so __DST_ENV_RANDOM_FILE__=random_node2.txt NODE_NAME=node2 "$bin_path"/atomic_server -reuse_addr -port=8302 -conf="127.0.1.1:8300:0,127.0.1.1:8301:0,127.0.1.1:8302:0," -data_path="./data2" > log2 2>&1 &
