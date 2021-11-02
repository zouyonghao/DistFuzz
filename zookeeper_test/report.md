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

2*. Severe unrecoverable error
    ```
    2021-11-01 10:55:41,215 [myid:4] - ERROR [SyncThread:4:ZooKeeperCriticalThread@49] - Severe unrecoverable error, from thread : SyncThread:4
    java.io.IOException: Write error
        at java.base/java.io.FileOutputStream.writeBytes(Native Method)
        at java.base/java.io.FileOutputStream.write(FileOutputStream.java:354)
        at java.base/java.io.BufferedOutputStream.flushBuffer(BufferedOutputStream.java:81)
        at java.base/java.io.BufferedOutputStream.flush(BufferedOutputStream.java:142)
        at org.apache.zookeeper.server.persistence.FileTxnLog.commit(FileTxnLog.java:377)
        at org.apache.zookeeper.server.persistence.FileTxnSnapLog.commit(FileTxnSnapLog.java:599)
        at org.apache.zookeeper.server.ZKDatabase.commit(ZKDatabase.java:657)
        at org.apache.zookeeper.server.SyncRequestProcessor.flush(SyncRequestProcessor.java:235)
        at org.apache.zookeeper.server.SyncRequestProcessor.run(SyncRequestProcessor.java:169)
    ```

    该错误会导致server崩溃，但是它是正常退出的，所以可能不是bug?

    issues:
    * https://issues.apache.org/jira/browse/ZOOKEEPER-2247
    * https://issues.apache.org/jira/browse/ZOOKEEPER-4408

3. NullPointerException in SendAckRequestProcessor
    ```
    java.lang.NullPointerException
        at org.apache.zookeeper.server.quorum.SendAckRequestProcessor.flush(SendAckRequestProcessor.java:67)
        at org.apache.zookeeper.server.SyncRequestProcessor.flush(SyncRequestProcessor.java:248)
        at org.apache.zookeeper.server.SyncRequestProcessor.run(SyncRequestProcessor.java:169)
    ```

    issues
    * https://issues.apache.org/jira/browse/ZOOKEEPER-4409
    * https://github.com/apache/zookeeper/pull/1774
