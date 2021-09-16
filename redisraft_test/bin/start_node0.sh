bin_path=/home/zyh/redis/src
lib_path=/home/zyh/redisraft

LD_PRELOAD=/home/zyh/distributed-system-test/build/preload_module/libdst_preload.so \
__DST_ENV_RANDOM_FILE__=random_node0.txt $bin_path/redis-server --bind 127.0.1.1 --port 5001 --dbfilename raft1.rdb --loadmodule $lib_path/redisraft.so raft-log-filename raftlog1.db addr 127.0.1.1:5001 >> redis1.log 2>&1 &
