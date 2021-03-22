bin_path=/home/zyh/braft/example/atomic

./start_node0.sh
./start_node1.sh
# ./atomic_server -port=8302 -conf="127.0.1.1:8300:0,127.0.1.1:8301:0,127.0.1.1:8302:0," -data_path="./data2" > log2 2>&1 &

# ./atomic_server -port=8303 -conf="127.0.1.1:8303:0,127.0.1.1:8304:0,127.0.1.1:8305:0," > log3 2>&1 &
./start_node2.sh
# ./start_proxy_server.sh
