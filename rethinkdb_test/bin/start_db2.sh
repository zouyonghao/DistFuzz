RETHINKDB_BUILD_PATH=/home/zyh/rethinkdb/build/release_clang_system/

# proxy 6000 to 5000
# proxy 6001 to 5001
# proxy 5002 to 6002
# ../../build/proxy_server/proxy_server ./proxy-config.json > log_proxy 2>&1 &
# ../../build/event_control/event_control > log_event_control 2>&1 &

LD_PRELOAD=/home/zyh/distributed-system-test/build/preload_module/libdst_preload.so \
__DST_ENV_RANDOM_FILE__=random_node2.txt ${RETHINKDB_BUILD_PATH}/rethinkdb --bind 127.0.1.1 --cluster-port 5002 \
        --driver-port 4002 --http-port 8082 -d ./rethinkdb_data2 \
        -j 127.0.1.1:5000 \
        -j 127.0.1.1:5001 >> log2 2>&1 &