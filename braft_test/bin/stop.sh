#!/bin/bash

killall atomic_server > /dev/null 2>&1
# ps -ef | grep atomic_server | awk '{print $2}' | xargs kill
# rm -rf data*
# rm -rf log*
# killall proxy_server
killall event_control > /dev/null 2>&1
# killall atomic_test

# ./clear.sh