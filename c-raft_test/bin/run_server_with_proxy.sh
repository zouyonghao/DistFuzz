CRAFT_SERVER_PATH=/home/zyh/raft/example

./mkdir_data_dir.sh
../../build/proxy_server/proxy_server ./proxy-config.json > log_proxy 2>&1 &
../../build/event_control/event_control > log_event_control 2>&1 &

# $CRAFT_SERVER_PATH/server `pwd`/data0 0  &
# $CRAFT_SERVER_PATH/server `pwd`/data1 1  &
# $CRAFT_SERVER_PATH/server `pwd`/data2 2  &

$CRAFT_SERVER_PATH/server ./c-raft_data0 1 9000 9000 9001 9002 &
$CRAFT_SERVER_PATH/server ./c-raft_data1 2 9001 9000 9001 9002 &
$CRAFT_SERVER_PATH/server ./c-raft_data2 3 9003 9004 9005 9003 &