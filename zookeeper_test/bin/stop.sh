ps -ef | grep QuorumPeerMain | grep -v grep | awk '{print $2}' | xargs kill -9
# rm -rf data*
# rm -rf log*
# killall proxy_server
killall event_control
