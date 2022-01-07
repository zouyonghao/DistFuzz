bin_path=/home/zyh/braft/example/atomic

mkdir data

for i in 0 1 2 3 4; do
    mkdir data/$i
done

setsid "$bin_path"/atomic_server -ip=127.0.0.1 -port=8300 -election_timeout_ms=100 \
    -conf="127.0.0.1:8300:0,127.0.0.1:8301:0,127.0.0.1:8302:0,127.0.0.1:8303:0,127.0.0.1:8304:0," \
    -data_path="./data/0" > ./data/log0 2>&1 &
setsid "$bin_path"/atomic_server -ip=127.0.0.1 -port=8301 -election_timeout_ms=100 \
    -conf="127.0.0.1:8300:0,127.0.0.1:8301:0,127.0.0.1:8302:0,127.0.0.1:8303:0,127.0.0.1:8304:0," \
    -data_path="./data/1" > ./data/log1 2>&1 &
setsid "$bin_path"/atomic_server -ip=127.0.0.1 -port=8302 -election_timeout_ms=100 \
    -conf="127.0.0.1:8300:0,127.0.0.1:8301:0,127.0.0.1:8302:0,127.0.0.1:8303:0,127.0.0.1:8304:0," \
    -data_path="./data/2" > ./data/log2 2>&1 &
setsid "$bin_path"/atomic_server -ip=127.0.0.1 -port=8303 -election_timeout_ms=100 \
    -conf="127.0.0.1:8300:0,127.0.0.1:8301:0,127.0.0.1:8302:0,127.0.0.1:8303:0,127.0.0.1:8304:0," \
    -data_path="./data/3" > ./data/log3 2>&1 &
setsid "$bin_path"/atomic_server -ip=127.0.0.1 -port=8304 -election_timeout_ms=100 \
    -conf="127.0.0.1:8300:0,127.0.0.1:8301:0,127.0.0.1:8302:0,127.0.0.1:8303:0,127.0.0.1:8304:0," \
    -data_path="./data/4" > ./data/log4 2>&1 &
