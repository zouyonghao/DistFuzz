bin_path=/home/zyh/redis/src
lib_path=/home/zyh/redisraft

mkdir data

cd data

setsid $bin_path/redis-server --bind 127.0.1.1 --port 5001 \
    --dbfilename raft1.rdb --loadmodule $lib_path/redisraft.so \
    election-timeout 300 raft-log-filename raftlog1.db addr 127.0.1.1:5001 >> redis1.log 2>&1 &

setsid $bin_path/redis-server --bind 127.0.1.1 --port 5002 \
    --dbfilename raft2.rdb --loadmodule $lib_path/redisraft.so \
    election-timeout 300 raft-log-filename raftlog2.db addr 127.0.1.1:5002 >> redis2.log 2>&1 &

setsid $bin_path/redis-server --bind 127.0.1.1 --port 5003 \
    --dbfilename raft3.rdb --loadmodule $lib_path/redisraft.so \
    election-timeout 300 raft-log-filename raftlog3.db addr 127.0.1.1:5003 >> redis3.log 2>&1 &

setsid $bin_path/redis-server --bind 127.0.1.1 --port 5004 \
    --dbfilename raft4.rdb --loadmodule $lib_path/redisraft.so \
    election-timeout 300 raft-log-filename raftlog4.db addr 127.0.1.1:5004 >> redis4.log 2>&1 &

setsid $bin_path/redis-server --bind 127.0.1.1 --port 5005 \
    --dbfilename raft5.rdb --loadmodule $lib_path/redisraft.so \
    election-timeout 300 raft-log-filename raftlog5.db addr 127.0.1.1:5005 >> redis5.log 2>&1 &

# sleep 1

for ((;;)) do redis-cli -h 127.0.1.1 -p 5001 raft.cluster init && break; done
for ((;;)) do redis-cli -h 127.0.1.1 -p 5002 RAFT.CLUSTER JOIN 127.0.1.1:5001 && break; done
for ((;;)) do redis-cli -h 127.0.1.1 -p 5003 RAFT.CLUSTER JOIN 127.0.1.1:5001 && break; done
for ((;;)) do redis-cli -h 127.0.1.1 -p 5004 RAFT.CLUSTER JOIN 127.0.1.1:5001 && break; done
for ((;;)) do redis-cli -h 127.0.1.1 -p 5005 RAFT.CLUSTER JOIN 127.0.1.1:5001 && break; done

ps -ef | grep redis-server | grep -v grep | grep 5001 | awk '{print $2}' | xargs sudo kill -9

setsid $bin_path/redis-server --bind 127.0.1.1 --port 5001 \
    --dbfilename raft1.rdb --loadmodule $lib_path/redisraft.so \
    election-timeout 300 raft-log-filename raftlog1.db addr 127.0.1.1:5001 >> redis1.log 2>&1 &

cd ..
