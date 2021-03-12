bin_path=/home/zyh/braft/example/atomic

"$bin_path"/atomic_server -reuse_addr -reuse_port -election_timeout_ms=5000 -port=8300 -conf="127.0.1.1:8300:0,127.0.1.1:8301:0,127.0.1.1:8302:0," -data_path="./data1" >> log1 2>&1 &
"$bin_path"/atomic_server -reuse_addr -reuse_port -election_timeout_ms=4000 -port=8301 -conf="127.0.1.1:8300:0,127.0.1.1:8301:0,127.0.1.1:8302:0," -data_path="./data2" >> log2 2>&1 &
# ./atomic_server -port=8302 -conf="127.0.1.1:8300:0,127.0.1.1:8301:0,127.0.1.1:8302:0," -data_path="./data2" > log2 2>&1 &

# ./atomic_server -port=8303 -conf="127.0.1.1:8303:0,127.0.1.1:8304:0,127.0.1.1:8305:0," > log3 2>&1 &
"$bin_path"/atomic_server -reuse_addr -reuse_port -election_timeout_ms=2000 -port=8303 -conf="127.0.1.1:8304:0,127.0.1.1:8305:0,127.0.1.1:8303:0," -data_path="./data3" >> log3 2>&1 &

# ./start_proxy_server.sh
