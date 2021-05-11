
./start_event_control.sh
./start_proxy_server.sh

./start_node0.sh
./start_node1.sh
./start_node2.sh

sleep 3

redis-cli -p 5001 raft.cluster init
redis-cli -p 5002 RAFT.CLUSTER JOIN localhost:5001
redis-cli -p 5004 RAFT.CLUSTER JOIN localhost:5005