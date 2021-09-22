#!/bin/bash

killall calc_server
ps -ef | grep calc_server | awk '{print $2}' | xargs kill
# rm -rf data*
# rm -rf log*
killall proxy_server
killall event_control
# killall atomic_test

# rm calc1.pipe
# rm calc2.pipe
# rm calc3.pipe

# ./clear.sh