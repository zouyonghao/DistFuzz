rm -rf hadoop
mkdir hadoop
./stop.sh > /dev/null 2>&1
./start_name_node.sh > /dev/null 2>&1 &
./start_event_control.sh
sleep 1