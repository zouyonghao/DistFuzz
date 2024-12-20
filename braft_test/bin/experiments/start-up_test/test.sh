g++ get_time.cpp -o get_time
./get_time

bin_path=/home/zyh/braft/example/atomic

"$bin_path"/atomic_server -reuse_addr -ip=127.0.1.1 -port=8300 -election_timeout_ms=100 -conf="127.0.1.1:8300:0,127.0.1.1:8301:0,127.0.1.1:8302:0,127.0.1.1:8303:0,127.0.1.1:8304:0," -data_path="./data0" > log0 2>&1 &
"$bin_path"/atomic_server -reuse_addr -ip=127.0.1.1 -port=8301 -election_timeout_ms=100 -conf="127.0.1.1:8300:0,127.0.1.1:8301:0,127.0.1.1:8302:0,127.0.1.1:8303:0,127.0.1.1:8304:0," -data_path="./data1" > log1 2>&1 &
"$bin_path"/atomic_server -reuse_addr -ip=127.0.1.1 -port=8302 -election_timeout_ms=100 -conf="127.0.1.1:8300:0,127.0.1.1:8301:0,127.0.1.1:8302:0,127.0.1.1:8303:0,127.0.1.1:8304:0," -data_path="./data2" > log2 2>&1 &
"$bin_path"/atomic_server -reuse_addr -ip=127.0.1.1 -port=8303 -election_timeout_ms=100 -conf="127.0.1.1:8300:0,127.0.1.1:8301:0,127.0.1.1:8302:0,127.0.1.1:8303:0,127.0.1.1:8304:0," -data_path="./data3" > log3 2>&1 &
"$bin_path"/atomic_server -reuse_addr -ip=127.0.1.1 -port=8304 -election_timeout_ms=100 -conf="127.0.1.1:8300:0,127.0.1.1:8301:0,127.0.1.1:8302:0,127.0.1.1:8303:0,127.0.1.1:8304:0," -data_path="./data4" > log4 2>&1 &

/home/zyh/braft/example/atomic/atomic_test -conf="127.0.1.1:8300:0,127.0.1.1:8301:0,127.0.1.1:8302:0,127.0.1.1:8303:0,127.0.1.1:8304:0," -atomic_op=get

./get_time

killall atomic_server
rm -rf data*
rm -rf log*
