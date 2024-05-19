mkdir data

for i in 1 2 3 4 5; do
    mkdir -p data/$i
    echo $i > data/$i/myid
done

setsid /usr/bin/java -Xmx16M -Xms16M -XX:MaxPermSize=16M -XX:-UsePerfData -server -cp /home/zyh/zookeeper/lib/zookeeper.jar:/home/zyh/zookeeper/lib/*:/home/zyh/zookeeper/conf \
  org.apache.zookeeper.server.quorum.QuorumPeerMain zoo1 > data/log1 2>&1 &
setsid /usr/bin/java -Xmx16M -Xms16M -XX:MaxPermSize=16M -XX:-UsePerfData -server -cp /home/zyh/zookeeper/lib/zookeeper.jar:/home/zyh/zookeeper/lib/*:/home/zyh/zookeeper/conf \
  org.apache.zookeeper.server.quorum.QuorumPeerMain zoo2 > data/log2 2>&1 &
setsid /usr/bin/java -Xmx16M -Xms16M -XX:MaxPermSize=16M -XX:-UsePerfData -server -cp /home/zyh/zookeeper/lib/zookeeper.jar:/home/zyh/zookeeper/lib/*:/home/zyh/zookeeper/conf \
  org.apache.zookeeper.server.quorum.QuorumPeerMain zoo3 > data/log3 2>&1 &
setsid /usr/bin/java -Xmx16M -Xms16M -XX:MaxPermSize=16M -XX:-UsePerfData -server -cp /home/zyh/zookeeper/lib/zookeeper.jar:/home/zyh/zookeeper/lib/*:/home/zyh/zookeeper/conf \
  org.apache.zookeeper.server.quorum.QuorumPeerMain zoo4 > data/log4 2>&1 &
setsid /usr/bin/java -Xmx16M -Xms16M -XX:MaxPermSize=16M -XX:-UsePerfData -server -cp /home/zyh/zookeeper/lib/zookeeper.jar:/home/zyh/zookeeper/lib/*:/home/zyh/zookeeper/conf \
  org.apache.zookeeper.server.quorum.QuorumPeerMain zoo5 > data/log5 2>&1 &