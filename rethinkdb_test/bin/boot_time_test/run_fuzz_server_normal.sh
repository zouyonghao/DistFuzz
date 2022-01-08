mkdir data

for i in 0 1 2 3 4; do
    mkdir data/$i
done

export RETHINKDB_BUILD_PATH=/home/zyh/rethinkdb/build/release_clang_system

setsid ${RETHINKDB_BUILD_PATH}/rethinkdb --bind 127.0.1.1 --cluster-port 5000 \
        --driver-port 4000 --http-port 8080 -d ./data/0 \
        -j 127.0.1.1:5001 -j 127.0.1.1:5002 -j 127.0.1.1:5003 -j 127.0.1.1:5004 >> data/log0 2>&1 &

setsid ${RETHINKDB_BUILD_PATH}/rethinkdb --bind 127.0.1.1 --cluster-port 5001 \
        --driver-port 4001 --http-port 8081 -d ./data/1 \
        -j 127.0.1.1:5000 -j 127.0.1.1:5002 -j 127.0.1.1:5003 -j 127.0.1.1:5004 >> data/log1 2>&1 &

setsid ${RETHINKDB_BUILD_PATH}/rethinkdb --bind 127.0.1.1 --cluster-port 5002 \
        --driver-port 4002 --http-port 8082 -d ./data/2 \
        -j 127.0.1.1:5000 -j 127.0.1.1:5001 -j 127.0.1.1:5003 -j 127.0.1.1:5004 >> data/log2 2>&1 &

setsid ${RETHINKDB_BUILD_PATH}/rethinkdb --bind 127.0.1.1 --cluster-port 5003 \
        --driver-port 4003 --http-port 8083 -d ./data/3 \
        -j 127.0.1.1:5000 -j 127.0.1.1:5001 -j 127.0.1.1:5002 -j 127.0.1.1:5004 >> data/log3 2>&1 &

setsid ${RETHINKDB_BUILD_PATH}/rethinkdb --bind 127.0.1.1 --cluster-port 5004 \
        --driver-port 4004 --http-port 8084 -d ./data/4 \
        -j 127.0.1.1:5000 -j 127.0.1.1:5001 -j 127.0.1.1:5002 -j 127.0.1.1:5003 >> data/log4 2>&1 &