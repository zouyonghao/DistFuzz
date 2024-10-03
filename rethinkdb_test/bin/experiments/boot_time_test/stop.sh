#!/bin/bash

killall rethinkdb > /dev/null 2>&1
ps -ef | grep criu | grep -v grep | grep -v sudo | awk '{print $2}' | xargs sudo kill -9 > /dev/null 2>&1
