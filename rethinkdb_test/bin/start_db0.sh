RETHINKDB_BUILD_PATH=/home/zyh/rethinkdb/build/release_clang_system

LD_PRELOAD=/home/zyh/distributed-system-test/build/preload_module/libdst_preload.so \
__DST_ENV_RANDOM_FILE__=random_node0.txt ${RETHINKDB_BUILD_PATH}/rethinkdb --bind 127.0.1.1 --cluster-port 5000 \
        --driver-port 4000 --http-port 8080 -d ./rethinkdb_data0 \
        -j 127.0.1.1:5001 -j 127.0.1.1:5002 \
        >> log0 2>&1 &
# ${RETHINKDB_BUILD_PATH}/rethinkdb --bind 127.0.0.1 --cluster-port 5000 \
#         --driver-port 4000 --http-port 8080 -d ./rethinkdb_data0 \
#         -j 127.0.0.1:5001 -j 127.0.0.1:5002 \
#         > log0 2>&1 &