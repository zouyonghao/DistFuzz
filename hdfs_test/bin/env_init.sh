rm -rf hadoop
./stop.sh > /dev/null 2>&1
./start_name_node.sh > /dev/null 2>&1 &
sleep 1