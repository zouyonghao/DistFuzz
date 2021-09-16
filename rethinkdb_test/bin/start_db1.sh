RETHINKDB_BUILD_PATH=/home/zyh/rethinkdb/build/release_clang_system/

LD_PRELOAD=/home/zyh/distributed-system-test/build/preload_module/libdst_preload.so \
__DST_ENV_RANDOM_FILE__=random_node1.txt ${RETHINKDB_BUILD_PATH}/rethinkdb --bind 127.0.1.1 --cluster-port 5001 \
        --driver-port 4001 --http-port 8081 -d ./rethinkdb_data1 \
        -j 127.0.1.1:5000 -j 127.0.1.1:5002 >> log1 2>&1 &
