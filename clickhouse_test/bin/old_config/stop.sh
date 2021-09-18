#!/bin/bash

# Copyright (c) 2018 Baidu.com, Inc. All Rights Reserved
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
#     http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# source shflags from current directory

killall clickhouse-server
# ps -ef | grep clickhouse-server | grep -v grep | awk '{print $2}' | xargs kill -9
# rm -rf data*
# rm -rf log*
killall proxy_server
killall event_control
./zkServer.sh stop
ps -ef | grep java | grep zookeeper | awk '{print $2}' | xargs kill -9

ps -ef | grep criu | grep -v grep | grep -v sudo | awk '{print $2}' | xargs sudo kill -9
