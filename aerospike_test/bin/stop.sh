#!/bin/bash
killall asd
ps -ef | grep asd | grep -v grep | awk '{print $2}' | xargs kill -9 > /dev/null 2>&1
# rm -rf data*
# rm -rf log*
# killall proxy_server
killall event_control
# killall atomic_test

# rm calc1.pipe
# rm calc2.pipe
# rm calc3.pipe

# ./clear.sh
