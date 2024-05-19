export HADOOP_HOME=/home/zouyo/hadoop
export HADOOP_CONF_DIR=./namenode
export PATH=$PATH:$HADOOP_HOME/bin:$JAVA_HOME/bin

rm hadoop/hadoop.log

if [ ! -d hadoop ]; then
java -cp \
    $HADOOP_CONF_DIR:$HADOOP_HOME/share/hadoop/common/*:$HADOOP_HOME/share/hadoop/common/lib/*:/home/zouyo/hadoop/share/hadoop/hdfs/*:/home/zouyo/hadoop/share/hadoop/hdfs/lib/* \
    org.apache.hadoop.hdfs.server.namenode.NameNode -format
fi

java -cp \
    $HADOOP_CONF_DIR:$HADOOP_HOME/share/hadoop/common/*:$HADOOP_HOME/share/hadoop/common/lib/*:/home/zouyo/hadoop/share/hadoop/hdfs/*:/home/zouyo/hadoop/share/hadoop/hdfs/lib/* \
    org.apache.hadoop.hdfs.server.namenode.NameNode
