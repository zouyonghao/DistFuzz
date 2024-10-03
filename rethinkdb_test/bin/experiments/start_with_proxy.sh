# export RETHINKDB_BUILD_PATH=/home/zyh/rethinkdb/build/release_clang_system_with_instrument
# export RETHINKDB_BUILD_PATH=/home/zyh/rethinkdb/build/release_clang_system_normal
export RETHINKDB_BUILD_PATH=/home/zyh/rethinkdb/build/release_clang_system

./start_db0.sh
./start_db1.sh

# proxy 6000 to 5000
# proxy 6001 to 5001
# proxy 5002 to 6002
../../build/proxy_server/proxy_server ./proxy-config.json > log_proxy 2>&1 &
../../build/event_control/event_control > log_event_control 2>&1 &

./start_db2.sh
