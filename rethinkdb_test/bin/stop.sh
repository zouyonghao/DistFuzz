killall rethinkdb -9
# killall proxy_server
killall event_control

rm -rf rethinkdb_data*
sudo tunctl -d tap0