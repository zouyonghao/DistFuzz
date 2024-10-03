# export RETHINKDB_BUILD_PATH=/home/zyh/rethinkdb/build/release_clang_system_with_instrument
# export RETHINKDB_BUILD_PATH=/home/zyh/rethinkdb/build/release_clang_system_normal
export RETHINKDB_BUILD_PATH=/home/zyh/rethinkdb/build/release_clang_system

# ../../build/event_control/event_control > log_event_control 2>&1 &

LD_PRELOAD=/home/zyh/DistFuzz/build/preload_module/libdst_preload.so \
${RETHINKDB_BUILD_PATH}/rethinkdb --bind 127.0.1.1 --cluster-port 5000 \
        --driver-port 4000 --http-port 8080 -d ./rethinkdb_data0 \
        -j 127.0.1.1:5001 -j 127.0.1.1:5002 \
        >> log0 2>&1 &
# sleep 1

LD_PRELOAD=/home/zyh/DistFuzz/build/preload_module/libdst_preload.so \
${RETHINKDB_BUILD_PATH}/rethinkdb --bind 127.0.1.1 --cluster-port 5001 \
        --driver-port 4001 --http-port 8081 -d ./rethinkdb_data1 \
        -j 127.0.1.1:5000 -j 127.0.1.1:5002 >> log1 2>&1 &
# sleep 1

LD_PRELOAD=/home/zyh/DistFuzz/build/preload_module/libdst_preload.so \
${RETHINKDB_BUILD_PATH}/rethinkdb --bind 127.0.1.1 --cluster-port 5002 \
        --driver-port 4002 --http-port 8082 -d ./rethinkdb_data2 \
        -j 127.0.1.1:5000 \
        -j 127.0.1.1:5001 >> log2 2>&1 &