export HADOOP_HOME=/home/zyh/hadoop
export PATH=$PATH:$HADOOP_HOME/bin:$JAVA_HOME/bin

rm hadoop/hadoop.log

hdfs journalnode -Ddfs.journalnode.rpc-address=localhost:8485 -Ddfs.journalnode.http-address=localhost:8488 -Ddfs.journalnode.edits.dir=/home/zyh/distributed-system-test/hdfs_test/bin/hadoop/dfs/journalnode1 &
hdfs journalnode -Ddfs.journalnode.rpc-address=localhost:8486 -Ddfs.journalnode.http-address=localhost:8489 -Ddfs.journalnode.edits.dir=/home/zyh/distributed-system-test/hdfs_test/bin/hadoop/dfs/journalnode2 &
hdfs journalnode -Ddfs.journalnode.rpc-address=localhost:8487 -Ddfs.journalnode.http-address=localhost:8490 -Ddfs.journalnode.edits.dir=/home/zyh/distributed-system-test/hdfs_test/bin/hadoop/dfs/journalnode3 &

export HADOOP_CONF_DIR=./namenode1
java -cp \
    $HADOOP_CONF_DIR:$HADOOP_HOME/share/hadoop/common/*:$HADOOP_HOME/share/hadoop/common/lib/*:/home/zyh/hadoop/share/hadoop/hdfs/*:/home/zyh/hadoop/share/hadoop/hdfs/lib/* \
    org.apache.hadoop.hdfs.server.namenode.NameNode -format -force -nonInteractive -clusterId mycluster

java -cp \
    $HADOOP_CONF_DIR:$HADOOP_HOME/share/hadoop/common/*:$HADOOP_HOME/share/hadoop/common/lib/*:/home/zyh/hadoop/share/hadoop/hdfs/*:/home/zyh/hadoop/share/hadoop/hdfs/lib/* \
    org.apache.hadoop.hdfs.server.namenode.NameNode &

$HADOOP_HOME/bin/hdfs zkfc &

export HADOOP_CONF_DIR=./namenode2
java -cp \
    $HADOOP_CONF_DIR:$HADOOP_HOME/share/hadoop/common/*:$HADOOP_HOME/share/hadoop/common/lib/*:/home/zyh/hadoop/share/hadoop/hdfs/*:/home/zyh/hadoop/share/hadoop/hdfs/lib/* \
    org.apache.hadoop.hdfs.server.namenode.NameNode -format -force -nonInteractive -clusterId mycluster

java -cp \
    $HADOOP_CONF_DIR:$HADOOP_HOME/share/hadoop/common/*:$HADOOP_HOME/share/hadoop/common/lib/*:/home/zyh/hadoop/share/hadoop/hdfs/*:/home/zyh/hadoop/share/hadoop/hdfs/lib/* \
    org.apache.hadoop.hdfs.server.namenode.NameNode &

$HADOOP_HOME/bin/hdfs zkfc &