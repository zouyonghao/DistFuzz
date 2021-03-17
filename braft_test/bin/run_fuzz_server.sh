bin_path=/home/zyh/braft/example/atomic

__DST_ENV_RANDOM_FILE__=random_node1.txt NODE_NAME=node1 "$bin_path"/atomic_server -reuse_addr -reuse_port -port=8300 -conf="127.0.1.1:8300:0,127.0.1.1:8301:0,127.0.1.1:8302:0," -data_path="./data1" >> log1 2>&1 &
__DST_ENV_RANDOM_FILE__=random_node2.txt NODE_NAME=node2 "$bin_path"/atomic_server -reuse_addr -reuse_port -port=8301 -conf="127.0.1.1:8300:0,127.0.1.1:8301:0,127.0.1.1:8302:0," -data_path="./data2" >> log2 2>&1 &
# ./atomic_server -port=8302 -conf="127.0.1.1:8300:0,127.0.1.1:8301:0,127.0.1.1:8302:0," -data_path="./data2" > log2 2>&1 &

# ./atomic_server -port=8303 -conf="127.0.1.1:8303:0,127.0.1.1:8304:0,127.0.1.1:8305:0," > log3 2>&1 &
__DST_ENV_RANDOM_FILE__=random_node3.txt NODE_NAME=node3 "$bin_path"/atomic_server -reuse_addr -reuse_port -port=8303 -conf="127.0.1.1:8304:0,127.0.1.1:8305:0,127.0.1.1:8303:0," -data_path="./data3" >> log3 2>&1 &

# ./start_proxy_server.sh
