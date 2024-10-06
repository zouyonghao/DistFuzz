
./start_event_control.sh
# ./start_proxy_server.sh

./start_node0.sh
./start_node1.sh
./start_node2.sh

sleep 3

redis-cli -h 127.0.1.1 -p 5001 raft.cluster init
redis-cli -h 127.0.1.1 -p 5002 RAFT.CLUSTER JOIN 127.0.1.1:5001
redis-cli -h 127.0.1.1 -p 5003 RAFT.CLUSTER JOIN 127.0.1.1:5001
# redis-cli -p 5003 RAFT.CLUSTER JOIN 127.0.1.1:5001