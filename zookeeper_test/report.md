1. Unable to load database on disk
   ```
    2021-11-01 10:38:34,848 [myid:4] - ERROR [main:QuorumPeer@1191] - Unable to load database on disk
    java.io.IOException: The accepted epoch, 9 is less than the current epoch, a
        at org.apache.zookeeper.server.quorum.QuorumPeer.loadDataBase(QuorumPeer.java:1188)
        at org.apache.zookeeper.server.quorum.QuorumPeer.start(QuorumPeer.java:1130)
        at org.apache.zookeeper.server.quorum.QuorumPeerMain.runFromConfig(QuorumPeerMain.java:229)
        at org.apache.zookeeper.server.quorum.QuorumPeerMain.initializeAndRun(QuorumPeerMain.java:137)
        at org.apache.zookeeper.server.quorum.QuorumPeerMain.main(QuorumPeerMain.java:91)
    2021-11-01 10:38:34,849 [myid:4] - ERROR [main:QuorumPeerMain@114] - Unexpected exception, exiting abnormally
    java.lang.RuntimeException: Unable to run quorum server 
        at org.apache.zookeeper.server.quorum.QuorumPeer.loadDataBase(QuorumPeer.java:1192)
        at org.apache.zookeeper.server.quorum.QuorumPeer.start(QuorumPeer.java:1130)
        at org.apache.zookeeper.server.quorum.QuorumPeerMain.runFromConfig(QuorumPeerMain.java:229)
        at org.apache.zookeeper.server.quorum.QuorumPeerMain.initializeAndRun(QuorumPeerMain.java:137)
        at org.apache.zookeeper.server.quorum.QuorumPeerMain.main(QuorumPeerMain.java:91)
    Caused by: java.io.IOException: The accepted epoch, 9 is less than the current epoch, a
        at org.apache.zookeeper.server.quorum.QuorumPeer.loadDataBase(QuorumPeer.java:1188)
        ... 4 more
   ```

    issues:
    * https://issues.apache.org/jira/browse/ZOOKEEPER-2307