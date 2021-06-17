
./start_event_control.sh
./start_proxy_server.sh

mkdir -p run/zookeeper

./zkServer.sh start

./start_node0.sh
./start_node1.sh
./start_node2.sh

./env_init.sh
