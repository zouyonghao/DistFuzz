CRAFT_SERVER_PATH=/home/zyh/raft/example

./mkdir_data_dir.sh

# $CRAFT_SERVER_PATH/server `pwd`/data0 1  &
# $CRAFT_SERVER_PATH/server `pwd`/data1 2  &
# $CRAFT_SERVER_PATH/server `pwd`/data2 3  &

$CRAFT_SERVER_PATH/server `pwd`/c-raft_data0 1 9000 9000 9001 9002  &
$CRAFT_SERVER_PATH/server `pwd`/c-raft_data1 2 9001 9000 9001 9002  &
$CRAFT_SERVER_PATH/server `pwd`/c-raft_data2 3 9002 9000 9001 9002  &