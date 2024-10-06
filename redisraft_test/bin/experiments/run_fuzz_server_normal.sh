bin_path=/home/zyh/redis/src
lib_path=/home/zyh/redisraft

LD_PRELOAD=/home/zyh/DistFuzz/build/preload_module/libdst_preload.so \
$bin_path/redis-server --bind 127.0.1.1 --port 5001 --dbfilename raft1.rdb --loadmodule $lib_path/redisraft.so raft-log-filename raftlog1.db addr 127.0.1.1:5001 >> redis1.log 2>&1 &

LD_PRELOAD=/home/zyh/DistFuzz/build/preload_module/libdst_preload.so \
$bin_path/redis-server --bind 127.0.1.1 --port 5002 --dbfilename raft2.rdb --loadmodule $lib_path/redisraft.so raft-log-filename raftlog2.db addr 127.0.1.1:5002 >> redis2.log 2>&1 &

LD_PRELOAD=/home/zyh/DistFuzz/build/preload_module/libdst_preload.so \
$bin_path/redis-server --bind 127.0.1.1 --port 5003 --dbfilename raft3.rdb --loadmodule $lib_path/redisraft.so raft-log-filename raftlog3.db addr 127.0.1.1:5003 >> redis3.log 2>&1 &

sleep 3

redis-cli -h 127.0.1.1 -p 5001 raft.cluster init
redis-cli -h 127.0.1.1 -p 5002 RAFT.CLUSTER JOIN 127.0.1.1:5001
redis-cli -h 127.0.1.1 -p 5003 RAFT.CLUSTER JOIN 127.0.1.1:5001