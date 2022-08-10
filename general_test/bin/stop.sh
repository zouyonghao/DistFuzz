# ps -ef | grep QuorumPeerMain | grep -v grep | awk '{print $2}' | xargs kill -9 > /dev/null 2>&1
killall event_control > /dev/null 2>&1
