./start_event_control.sh
./start_proxy_server.sh

mkdir -p run/zookeeper

./zkServer.sh start

./restore_checkpoint_tmpfs.sh

./ensure_nodes.sh
