ps -ef | grep java | grep -v grep | grep -v bin/rr | awk '{print $2}' | xargs kill -9 > /dev/null 2>&1
# rm -rf data*
# rm -rf log*
# killall proxy_server
killall event_control > /dev/null 2>&1
