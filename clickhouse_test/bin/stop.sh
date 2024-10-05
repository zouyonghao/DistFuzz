killall clickhouse-keeper -9
killall clickhouse -9
# ps -ef | grep clickhouse-server | grep -v grep | awk '{print $2}' | xargs kill -9
# rm -rf data*
# rm -rf log*
# killall proxy_server
killall event_control
# ./zkServer.sh stop
# ps -ef | grep java | grep zookeeper | awk '{print $2}' | xargs kill -9
# ps -ef | grep criu | grep -v grep | grep -v sudo | awk '{print $2}' | xargs sudo kill -9
