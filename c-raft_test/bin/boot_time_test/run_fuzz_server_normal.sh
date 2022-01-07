CRAFT_SERVER_PATH=/home/zyh/raft/example

mkdir data

for i in 0 1 2 3 4; do
    mkdir data/$i
done

setsid $CRAFT_SERVER_PATH/server ./data/0 1 9000 9000 9001 9002 9003 9004 >./data/log0 2>&1 &
setsid $CRAFT_SERVER_PATH/server ./data/1 2 9001 9000 9001 9002 9003 9004 >./data/log1 2>&1 &
setsid $CRAFT_SERVER_PATH/server ./data/2 3 9002 9000 9001 9002 9003 9004 >./data/log2 2>&1 &
setsid $CRAFT_SERVER_PATH/server ./data/3 4 9003 9000 9001 9002 9003 9004 >./data/log3 2>&1 &
setsid $CRAFT_SERVER_PATH/server ./data/4 5 9004 9000 9001 9002 9003 9004 >./data/log4 2>&1 &
