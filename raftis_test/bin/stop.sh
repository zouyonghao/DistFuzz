#!/bin/bash

# pkill raftis
ps -ef | grep raftis | grep data | awk '{print $2}' | xargs kill
# rm -rf data*
# rm -rf log*
pkill proxy_server
pkill event_control

# ./clear.sh