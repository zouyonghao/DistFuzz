#!/bin/bash

killall redis-server
ps -ef | grep redis-server | grep -v grep | awk '{print $2}' | xargs kill -9
# rm -rf data*
# rm -rf log*
killall proxy_server
killall event_control
# killall atomic_test

# rm calc1.pipe
# rm calc2.pipe
# rm calc3.pipe

# ./clear.sh
