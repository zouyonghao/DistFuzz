SERVER_PATH=/home/zyh/go/bin/dqlite-demo

mkdir dqlite_data0
mkdir dqlite_data1
mkdir dqlite_data2


$SERVER_PATH -a 127.0.0.1:3000 -d 127.0.0.1:4000 -D dqlite_data0 &

$SERVER_PATH -a 127.0.0.1:3001 -d 127.0.0.1:4001 -D dqlite_data1 -j 127.0.0.1:4000 &

$SERVER_PATH -a 127.0.0.1:3002 -d 127.0.0.1:4002 -D dqlite_data2 -j 127.0.0.1:4000 &