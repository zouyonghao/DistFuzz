
bin_path=/home/zyh/braft/example/atomic

for ((;;))
do
./stop_proxy_server.sh
ps -ef | grep atomic_server | head -1 | awk '{print $2}' | xargs kill -9
sleep 10
$bin_path/atomic_server -port=8300 -conf="127.0.1.1:8300:0,127.0.1.1:8301:0,127.0.1.1:8302:0," -data_path="./data1" >> log1 2>&1 &
$bin_path/atomic_server -port=8301 -conf="127.0.1.1:8300:0,127.0.1.1:8301:0,127.0.1.1:8302:0," -data_path="./data2" >> log2 2>&1 &
$bin_path/atomic_server -port=8303 -conf="127.0.1.1:8304:0,127.0.1.1:8305:0,127.0.1.1:8303:0," -data_path="./data3" >> log3 2>&1 &
./start_proxy_server.sh
sleep 15
done