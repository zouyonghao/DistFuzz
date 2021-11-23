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

2. Severe unrecoverable error *

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

4. Committing zxid 0x100000003 but next pending txn 0x100000002
    ```
    2021-11-23 17:02:48,009 [myid:2] - INFO  [WorkerReceiver[myid=2]:FastLeaderElection$Messenger$WorkerReceiver@390] - Notification: my state:FOLLOWING; n.sid:4, n.state:LOOKING, n.leader:4, n.round:0x2, n.peerEpoch:0x1, n.zxid:0x0, message format version:0x2, n.config version:0x0
    2021-11-23 17:02:48,113 [myid:2] - WARN  [QuorumPeer[myid=2](plain=[0:0:0:0:0:0:0:0]:2182)(secure=disabled):Follower@172] - Got zxid 0x100000001 expected 0x1
    2021-11-23 17:02:48,114 [myid:2] - INFO  [SyncThread:2:FileTxnLog@284] - Creating new log file: log.100000001
    2021-11-23 17:02:48,127 [myid:2] - INFO  [CommitProcessor:2:LearnerSessionTracker@116] - Committing global session 0x1000b301f680000
    2021-11-23 17:02:48,155 [myid:2] - WARN  [QuorumPeer[myid=2](plain=[0:0:0:0:0:0:0:0]:2182)(secure=disabled):Follower@172] - Got zxid 0x100000002 expected 0x100000003
    2021-11-23 17:02:48,162 [myid:2] - WARN  [SyncThread:2:FileTxnLog@275] - Current zxid 4294967298 is <= 4294967298 for 1
    2021-11-23 17:02:48,724 [myid:2] - ERROR [QuorumPeer[myid=2](plain=[0:0:0:0:0:0:0:0]:2182)(secure=disabled):FollowerZooKeeperServer@104] - Committing zxid 0x100000003 but next pending txn 0x100000002
    2021-11-23 17:02:48,726 [myid:2] - ERROR [QuorumPeer[myid=2](plain=[0:0:0:0:0:0:0:0]:2182)(secure=disabled):ServiceUtils@42] - Exiting JVM with code 12
    ```

    issues
    * https://issues.apache.org/jira/browse/ZOOKEEPER-4418

    error logs
    * error_cases/8000