#!/bin/bash

# killall atomic_server
# ps -ef | grep atomic_server | awk '{print $2}' | xargs kill
# rm -rf data*
# rm -rf log*
# killall proxy_server
killall event_control
killall atomic_test

# ./clear.sh