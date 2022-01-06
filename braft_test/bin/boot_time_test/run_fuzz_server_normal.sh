bin_path=/home/zyh/braft/example/atomic

NODE_ID=0 "$bin_path"/atomic_server -reuse_addr -ip=127.0.1.1 -port=8300 -election_timeout_ms=100 -conf="127.0.1.1:8300:0,127.0.1.1:8301:0,127.0.1.1:8302:0," -data_path="./data0" > log0 2>&1 &
NODE_ID=1 "$bin_path"/atomic_server -reuse_addr -ip=127.0.1.1 -port=8301 -election_timeout_ms=100 -conf="127.0.1.1:8300:0,127.0.1.1:8301:0,127.0.1.1:8302:0," -data_path="./data1" > log1 2>&1 &
NODE_ID=2 "$bin_path"/atomic_server -reuse_addr -ip=127.0.1.1 -port=8302 -election_timeout_ms=100 -conf="127.0.1.1:8300:0,127.0.1.1:8301:0,127.0.1.1:8302:0," -data_path="./data2" > log2 2>&1 &
