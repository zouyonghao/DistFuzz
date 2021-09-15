
# mkfifo calc1.pipe
# mkfifo calc2.pipe
# mkfifo calc3.pipe
# sleep infinity > calc1.pipe &
# sleep infinity > calc2.pipe &
# sleep infinity > calc3.pipe &

./start_event_control.sh
./start_proxy_server.sh

./start_node0.sh
./start_node1.sh
./start_node2.sh

sleep 1
echo add 2 127.0.1.1:10002 > calc1.pipe
sleep 1
echo add 3 127.0.1.1:10003 > calc1.pipe

