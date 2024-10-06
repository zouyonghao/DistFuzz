# kill nn1 first
./start_name_node.sh
ps -ef | grep namenode1 | grep -v grep | grep -v bin/rr | awk '{print $2}' | xargs kill -9 > /dev/null 2>&1
HADOOP_CONF_DIR=./namenode2 ~/hadoop/bin/hdfs dfsadmin -fs hdfs://mycluster -safemode enter
# It should output ... ConnectRefused

./stop.sh