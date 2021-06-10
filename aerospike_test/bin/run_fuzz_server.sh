
./start_event_control.sh
./start_proxy_server.sh

mkdir -p run/log
mkdir -p run/work1/smd
mkdir -p run/work1/sys/udf/lua
mkdir -p run/work1/usr/udf/lua
mkdir -p run/work2/smd
mkdir -p run/work2/sys/udf/lua
mkdir -p run/work2/usr/udf/lua
mkdir -p run/work3/smd
mkdir -p run/work3/sys/udf/lua
mkdir -p run/work3/usr/udf/lua

./start_node0.sh
./start_node1.sh
./start_node2.sh
