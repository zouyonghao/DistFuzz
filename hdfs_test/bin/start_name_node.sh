export HADOOP_HOME=/home/zyh/hadoop
export PATH=$PATH:$HADOOP_HOME/bin:$JAVA_HOME/bin

rm hadoop/hadoop.log

hdfs journalnode -Ddfs.journalnode.rpc-address=127.0.0.1:8485 -Ddfs.journalnode.http-address=127.0.0.1:8488 -Ddfs.journalnode.edits.dir=/home/zyh/DistFuzz/hdfs_test/bin/hadoop/dfs/journalnode1 &
hdfs journalnode -Ddfs.journalnode.rpc-address=127.0.0.1:8486 -Ddfs.journalnode.http-address=127.0.0.1:8489 -Ddfs.journalnode.edits.dir=/home/zyh/DistFuzz/hdfs_test/bin/hadoop/dfs/journalnode2 &
hdfs journalnode -Ddfs.journalnode.rpc-address=127.0.0.1:8487 -Ddfs.journalnode.http-address=127.0.0.1:8490 -Ddfs.journalnode.edits.dir=/home/zyh/DistFuzz/hdfs_test/bin/hadoop/dfs/journalnode3 &

/home/zyh/zookeeper/bin/zkServer.sh start

sleep 2

export HADOOP_CONF_DIR=/home/zyh/DistFuzz/hdfs_test/bin/namenode1

java -cp \
    $HADOOP_CONF_DIR:$HADOOP_HOME/share/hadoop/common/*:$HADOOP_HOME/share/hadoop/common/lib/*:/home/zyh/hadoop/share/hadoop/hdfs/*:/home/zyh/hadoop/share/hadoop/hdfs/lib/* \
    org.apache.hadoop.hdfs.server.namenode.NameNode -format -force -nonInteractive -clusterId mycluster

java -cp \
    $HADOOP_CONF_DIR:$HADOOP_HOME/share/hadoop/common/*:$HADOOP_HOME/share/hadoop/common/lib/*:/home/zyh/hadoop/share/hadoop/hdfs/*:/home/zyh/hadoop/share/hadoop/hdfs/lib/* \
    org.apache.hadoop.hdfs.server.namenode.NameNode &

sleep 1

HADOOP_PID_DIR=./hadoop/hdfs/namenode1 $HADOOP_HOME/bin/hdfs zkfc -formatZK -force -nonInteractive
HADOOP_PID_DIR=./hadoop/hdfs/namenode1 $HADOOP_HOME/bin/hdfs zkfc &

export HADOOP_CONF_DIR=/home/zyh/DistFuzz/hdfs_test/bin/namenode2
java -cp \
    $HADOOP_CONF_DIR:$HADOOP_HOME/share/hadoop/common/*:$HADOOP_HOME/share/hadoop/common/lib/*:/home/zyh/hadoop/share/hadoop/hdfs/*:/home/zyh/hadoop/share/hadoop/hdfs/lib/* \
    org.apache.hadoop.hdfs.server.namenode.NameNode -format -force -nonInteractive -clusterId mycluster

java -cp \
    $HADOOP_CONF_DIR:$HADOOP_HOME/share/hadoop/common/*:$HADOOP_HOME/share/hadoop/common/lib/*:/home/zyh/hadoop/share/hadoop/hdfs/*:/home/zyh/hadoop/share/hadoop/hdfs/lib/* \
    org.apache.hadoop.hdfs.server.namenode.NameNode &

sleep 1

HADOOP_PID_DIR=./hadoop/hdfs/namenode2 $HADOOP_HOME/bin/hdfs zkfc &
