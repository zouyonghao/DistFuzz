killall rethinkdb -9
# killall proxy_server
killall event_control

rm -rf rethinkdb_data*
tunctl -d tap0