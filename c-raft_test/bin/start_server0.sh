CRAFT_SERVER_PATH=/home/zyh/raft/example

./mkdir_data_dir.sh

# $CRAFT_SERVER_PATH/server `pwd`/data0 0  &
# $CRAFT_SERVER_PATH/server `pwd`/data1 1  &
# $CRAFT_SERVER_PATH/server `pwd`/data2 2  &

__DST_ENV_RANDOM_FILE__=random_node0.txt $CRAFT_SERVER_PATH/server ./c-raft_data0 1 9000 9000 9001 9002 >> log0 2>&1 &
# $CRAFT_SERVER_PATH/server ./c-raft_data1 2 9001 9000 9001 9002 &
# $CRAFT_SERVER_PATH/server ./c-raft_data2 3 9003 9004 9005 9003 &