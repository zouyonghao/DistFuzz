bin_path=/home/zyh/redis/src
lib_path=/home/zyh/redisraft

# __DST_ENV_RANDOM_FILE__=random_node0.txt $bin_path/redis-server --port 5001 --dbfilename raft1.rdb --loadmodule $lib_path/redisraft.so raft-log-filename raftlog1.db addr localhost:5001 >> redis1.log 2>&1 &
__DST_ENV_RANDOM_FILE__=random_node1.txt $bin_path/redis-server --port 5002 --dbfilename raft2.rdb --loadmodule $lib_path/redisraft.so raft-log-filename raftlog2.db addr localhost:5002 >> redis2.log 2>&1 &