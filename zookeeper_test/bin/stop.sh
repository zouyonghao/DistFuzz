ps -ef | grep QuorumPeerMain | grep -v grep | grep -v rr | awk '{print $2}' | xargs kill -9 > /dev/null 2>&1
# rm -rf data*
# rm -rf log*
# killall proxy_server
killall event_control > /dev/null 2>&1
