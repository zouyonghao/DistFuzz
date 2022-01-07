#!/bin/bash

sudo killall -9 calc_server > /dev/null 2>&1
sudo killall sleep
# ps -ef | grep clickhouse-server | grep -v grep | awk '{print $2}' | xargs kill -9
# rm -rf data*
# rm -rf log*
# killall proxy_server
# killall event_control

ps -ef | grep criu | grep -v grep | grep -v sudo | awk '{print $2}' | xargs sudo kill -9 > /dev/null 2>&1

sleep 1
