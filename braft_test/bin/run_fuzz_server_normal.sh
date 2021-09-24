bin_path=/home/zyh/braft/example/atomic

LD_PRELOAD=/home/zyh/distributed-system-test/build/preload_module/libdst_preload.so NODE_NAME=node0 "$bin_path"/atomic_server -reuse_addr -ip=127.0.1.1 -port=8300 -election_timeout_ms=100 -conf="127.0.1.1:8300:0,127.0.1.1:8301:0,127.0.1.1:8302:0," -data_path="./data0" > log0 2>&1 &
LD_PRELOAD=/home/zyh/distributed-system-test/build/preload_module/libdst_preload.so NODE_NAME=node1 "$bin_path"/atomic_server -reuse_addr -ip=127.0.1.1 -port=8301 -election_timeout_ms=100 -conf="127.0.1.1:8300:0,127.0.1.1:8301:0,127.0.1.1:8302:0," -data_path="./data1" > log1 2>&1 &
LD_PRELOAD=/home/zyh/distributed-system-test/build/preload_module/libdst_preload.so NODE_NAME=node2 "$bin_path"/atomic_server -reuse_addr -ip=127.0.1.1 -port=8302 -election_timeout_ms=100 -conf="127.0.1.1:8300:0,127.0.1.1:8301:0,127.0.1.1:8302:0," -data_path="./data2" > log2 2>&1 &

# ./atomic_server -port=8303 -conf="127.0.1.1:8303:0,127.0.1.1:8304:0,127.0.1.1:8305:0," > log3 2>&1 &
# ./atomic_server -port=8303 -conf="127.0.1.1:8304:0,127.0.1.1:8305:0,127.0.1.1:8303:0," -data_path="./data3" > log3 2>&1 &

# ./start_proxy_server.sh