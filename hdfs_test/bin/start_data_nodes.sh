export JAVA_HOME=/usr/lib/jvm/java-11-openjdk-amd64
export HADOOP_HOME=/home/zyh/hadoop
export PATH=$PATH:$HADOOP_HOME/bin:$JAVA_HOME/bin

export HADOOP_CONF_DIR=./datanode1
java -cp \
    $HADOOP_CONF_DIR:$HADOOP_HOME/share/hadoop/common/*:$HADOOP_HOME/share/hadoop/common/lib/*:$HADOOP_HOME/share/hadoop/hdfs/*:$HADOOP_HOME/share/hadoop/hdfs/lib/* \
    org.apache.hadoop.hdfs.server.datanode.DataNode > /dev/null 2>&1 &

export HADOOP_CONF_DIR=./datanode2
java -cp \
    $HADOOP_CONF_DIR:$HADOOP_HOME/share/hadoop/common/*:$HADOOP_HOME/share/hadoop/common/lib/*:$HADOOP_HOME/share/hadoop/hdfs/*:$HADOOP_HOME/share/hadoop/hdfs/lib/* \
    org.apache.hadoop.hdfs.server.datanode.DataNode > /dev/null 2>&1 &

export HADOOP_CONF_DIR=./datanode3
java -cp \
    $HADOOP_CONF_DIR:$HADOOP_HOME/share/hadoop/common/*:$HADOOP_HOME/share/hadoop/common/lib/*:$HADOOP_HOME/share/hadoop/hdfs/*:$HADOOP_HOME/share/hadoop/hdfs/lib/* \
    org.apache.hadoop.hdfs.server.datanode.DataNode > /dev/null 2>&1 &
