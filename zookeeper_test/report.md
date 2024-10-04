### Unable to load database on disk
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

    issues
    * https://issues.apache.org/jira/browse/ZOOKEEPER-2307
    * https://issues.apache.org/jira/browse/ZOOKEEPER-4781

### Severe unrecoverable error *

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

    issues
    * https://issues.apache.org/jira/browse/ZOOKEEPER-2247
    * https://issues.apache.org/jira/browse/ZOOKEEPER-4408

### NullPointerException in SendAckRequestProcessor
    ```
    java.lang.NullPointerException
        at org.apache.zookeeper.server.quorum.SendAckRequestProcessor.flush(SendAckRequestProcessor.java:67)
        at org.apache.zookeeper.server.SyncRequestProcessor.flush(SyncRequestProcessor.java:248)
        at org.apache.zookeeper.server.SyncRequestProcessor.run(SyncRequestProcessor.java:169)
    ```

    issues
    * https://issues.apache.org/jira/browse/ZOOKEEPER-4409
    * https://github.com/apache/zookeeper/pull/1774
    * https://github.com/apache/zookeeper/pull/2163

    **Fixed**

### Committing zxid 0x100000003 but next pending txn 0x100000002
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

### ZooKeeperServer not running
    ```
    2021-11-23 13:16:36,113 [myid:3] - WARN  [NIOWorkerThread-4:NIOServerCnxn@380] - Close of session 0x0
    java.io.IOException: ZooKeeperServer not running
        at org.apache.zookeeper.server.NIOServerCnxn.readLength(NIOServerCnxn.java:554)
        at org.apache.zookeeper.server.NIOServerCnxn.doIO(NIOServerCnxn.java:339)
        at org.apache.zookeeper.server.NIOServerCnxnFactory$IOWorkRequest.doWork(NIOServerCnxnFactory.java:508)
        at org.apache.zookeeper.server.WorkerService$ScheduledWorkRequest.run(WorkerService.java:154)
        at java.util.concurrent.ThreadPoolExecutor.runWorker(ThreadPoolExecutor.java:1149)
        at java.util.concurrent.ThreadPoolExecutor$Worker.run(ThreadPoolExecutor.java:624)
        at java.lang.Thread.run(Thread.java:748)
    ```

    在结束错误注入之后，zookeeper短时间内未能恢复到可以接收请求的状态。

    issues
    * https://issues.apache.org/jira/browse/ZOOKEEPER-1330
    * https://github.com/pravega/zookeeper-operator/issues/393
    * https://github.com/apache/zookeeper/pull/1770

    **Fixed**

### Client fail
    ```
    2021-11-30 15:01:17,592 [myid:3] - INFO  [QuorumPeer[myid=3](plain=0.0.0.0:2183)(secure=disabled):QuorumPeer@1512] - FOLLOWING
    2021-11-30 15:01:17,593 [myid:3] - INFO  [QuorumPeer[myid=3](plain=0.0.0.0:2183)(secure=disabled):ZooKeeperServer@1300] - minSessionTimeout set to 400
    2021-11-30 15:01:17,594 [myid:3] - INFO  [QuorumPeer[myid=3](plain=0.0.0.0:2183)(secure=disabled):ZooKeeperServer@1309] - maxSessionTimeout set to 4000
    2021-11-30 15:01:17,595 [myid:3] - INFO  [QuorumPeer[myid=3](plain=0.0.0.0:2183)(secure=disabled):ResponseCache@45] - getData response cache size is initialized with value 400.
    2021-11-30 15:01:17,598 [myid:3] - INFO  [QuorumPeer[myid=3](plain=0.0.0.0:2183)(secure=disabled):ResponseCache@45] - getChildren response cache size is initialized with value 400.
    2021-11-30 15:01:17,602 [myid:3] - INFO  [QuorumPeer[myid=3](plain=0.0.0.0:2183)(secure=disabled):RequestPathMetricsCollector@109] - zookeeper.pathStats.slotCapacity = 60
    2021-11-30 15:01:17,604 [myid:3] - INFO  [QuorumPeer[myid=3](plain=0.0.0.0:2183)(secure=disabled):RequestPathMetricsCollector@110] - zookeeper.pathStats.slotDuration = 15
    2021-11-30 15:01:17,606 [myid:3] - INFO  [QuorumPeer[myid=3](plain=0.0.0.0:2183)(secure=disabled):RequestPathMetricsCollector@111] - zookeeper.pathStats.maxDepth = 6
    2021-11-30 15:01:17,607 [myid:3] - INFO  [QuorumPeer[myid=3](plain=0.0.0.0:2183)(secure=disabled):RequestPathMetricsCollector@112] - zookeeper.pathStats.initialDelay = 5
    2021-11-30 15:01:17,607 [myid:3] - INFO  [QuorumPeer[myid=3](plain=0.0.0.0:2183)(secure=disabled):RequestPathMetricsCollector@113] - zookeeper.pathStats.delay = 5
    2021-11-30 15:01:17,608 [myid:3] - INFO  [QuorumPeer[myid=3](plain=0.0.0.0:2183)(secure=disabled):RequestPathMetricsCollector@114] - zookeeper.pathStats.enabled = false
    2021-11-30 15:01:17,627 [myid:3] - INFO  [WorkerReceiver[myid=3]:FastLeaderElection$Messenger$WorkerReceiver@390] - Notification: my state:FOLLOWING; n.sid:2, n.state:LOOKING, n.leader:2, n.round:0x2, n.peerEpoch:0x1, n.zxid:0x100000002, message format version:0x2, n.config version:0x0
    2021-11-30 15:01:17,629 [myid:3] - INFO  [QuorumPeer[myid=3](plain=0.0.0.0:2183)(secure=disabled):ZooKeeperServer@1536] - The max bytes for all large requests are set to 104857600
    2021-11-30 15:01:17,630 [myid:3] - INFO  [QuorumPeer[myid=3](plain=0.0.0.0:2183)(secure=disabled):ZooKeeperServer@1550] - The large request threshold is set to -1
    2021-11-30 15:01:17,631 [myid:3] - INFO  [QuorumPeer[myid=3](plain=0.0.0.0:2183)(secure=disabled):AuthenticationHelper@66] - zookeeper.enforce.auth.enabled = false
    2021-11-30 15:01:17,631 [myid:3] - INFO  [QuorumPeer[myid=3](plain=0.0.0.0:2183)(secure=disabled):AuthenticationHelper@67] - zookeeper.enforce.auth.schemes = []
    2021-11-30 15:01:17,632 [myid:3] - INFO  [QuorumPeer[myid=3](plain=0.0.0.0:2183)(secure=disabled):ZooKeeperServer@361] - Created server with tickTime 200 minSessionTimeout 400 maxSessionTimeout 4000 clientPortListenBacklog -1 datadir data3/version-2 snapdir data3/version-2
    2021-11-30 15:01:17,632 [myid:3] - INFO  [QuorumPeer[myid=3](plain=0.0.0.0:2183)(secure=disabled):Follower@77] - FOLLOWING - LEADER ELECTION TOOK - 380 MS
    2021-11-30 15:01:17,633 [myid:3] - INFO  [QuorumPeer[myid=3](plain=0.0.0.0:2183)(secure=disabled):QuorumPeer@915] - Peer state changed: following - discovery
    2021-11-30 15:01:17,657 [myid:3] - INFO  [LeaderConnector-/127.0.1.1:2892:Learner$LeaderConnector@381] - Successfully connected to leader, using address: /127.0.1.1:2892
    2021-11-30 15:01:17,768 [myid:3] - INFO  [QuorumPeer[myid=3](plain=0.0.0.0:2183)(secure=disabled):QuorumPeer@915] - Peer state changed: following - synchronization
    2021-11-30 15:01:17,822 [myid:3] - INFO  [QuorumPeer[myid=3](plain=0.0.0.0:2183)(secure=disabled):Learner@562] - Getting a diff from the leader 0x100000002
    2021-11-30 15:01:17,822 [myid:3] - INFO  [QuorumPeer[myid=3](plain=0.0.0.0:2183)(secure=disabled):QuorumPeer@920] - Peer state changed: following - synchronization - diff
    2021-11-30 15:01:17,823 [myid:3] - INFO  [QuorumPeer[myid=3](plain=0.0.0.0:2183)(secure=disabled):Learner@734] - Learner received NEWLEADER message
    2021-11-30 15:01:17,824 [myid:3] - INFO  [QuorumPeer[myid=3](plain=0.0.0.0:2183)(secure=disabled):QuorumPeer@1867] - Dynamic reconfig is disabled, we don't store the last seen config.
    2021-11-30 15:01:17,828 [myid:3] - INFO  [QuorumPeer[myid=3](plain=0.0.0.0:2183)(secure=disabled):QuorumPeer@920] - Peer state changed: following - synchronization
    2021-11-30 15:01:17,830 [myid:3] - INFO  [QuorumPeer[myid=3](plain=0.0.0.0:2183)(secure=disabled):CommitProcessor@491] - Configuring CommitProcessor with readBatchSize -1 commitBatchSize 1
    2021-11-30 15:01:17,832 [myid:3] - INFO  [QuorumPeer[myid=3](plain=0.0.0.0:2183)(secure=disabled):CommitProcessor@452] - Configuring CommitProcessor with 8 worker threads.
    2021-11-30 15:01:17,839 [myid:3] - INFO  [QuorumPeer[myid=3](plain=0.0.0.0:2183)(secure=disabled):FollowerRequestProcessor@59] - Initialized FollowerRequestProcessor with zookeeper.follower.skipLearnerRequestToNextProcessor as false
    2021-11-30 15:01:17,866 [myid:3] - WARN  [QuorumPeer[myid=3](plain=0.0.0.0:2183)(secure=disabled):MBeanRegistry@110] - Failed to register MBean InMemoryDataTree
    2021-11-30 15:01:17,866 [myid:3] - WARN  [QuorumPeer[myid=3](plain=0.0.0.0:2183)(secure=disabled):LearnerZooKeeperServer@104] - Failed to register with JMX
    javax.management.InstanceAlreadyExistsException: org.apache.ZooKeeperService:name0=ReplicatedServer_id3,name1=replica.3,name2=Follower,name3=InMemoryDataTree
            at java.management/com.sun.jmx.mbeanserver.Repository.addMBean(Repository.java:436)
            at java.management/com.sun.jmx.interceptor.DefaultMBeanServerInterceptor.registerWithRepository(DefaultMBeanServerInterceptor.java:1855)
            at java.management/com.sun.jmx.interceptor.DefaultMBeanServerInterceptor.registerDynamicMBean(DefaultMBeanServerInterceptor.java:955)
            at java.management/com.sun.jmx.interceptor.DefaultMBeanServerInterceptor.registerObject(DefaultMBeanServerInterceptor.java:890)
            at java.management/com.sun.jmx.interceptor.DefaultMBeanServerInterceptor.registerMBean(DefaultMBeanServerInterceptor.java:320)
            at java.management/com.sun.jmx.mbeanserver.JmxMBeanServer.registerMBean(JmxMBeanServer.java:522)
            at org.apache.zookeeper.jmx.MBeanRegistry.register(MBeanRegistry.java:106)
            at org.apache.zookeeper.server.quorum.LearnerZooKeeperServer.registerJMX(LearnerZooKeeperServer.java:102)
            at org.apache.zookeeper.server.ZooKeeperServer.startupWithServerState(ZooKeeperServer.java:709)
            at org.apache.zookeeper.server.ZooKeeperServer.startupWithoutServing(ZooKeeperServer.java:692)
            at org.apache.zookeeper.server.quorum.Learner.syncWithLeader(Learner.java:757)
            at org.apache.zookeeper.server.quorum.Follower.followLeader(Follower.java:109)
            at org.apache.zookeeper.server.quorum.QuorumPeer.run(QuorumPeer.java:1514)
    2021-11-30 15:01:17,883 [myid:3] - INFO  [QuorumPeer[myid=3](plain=0.0.0.0:2183)(secure=disabled):Learner@718] - Learner received UPTODATE message
    2021-11-30 15:01:17,884 [myid:3] - INFO  [QuorumPeer[myid=3](plain=0.0.0.0:2183)(secure=disabled):QuorumPeer@915] - Peer state changed: following - broadcast
    2021-11-30 15:01:18,193 [myid:3] - WARN  [QuorumPeer[myid=3](plain=0.0.0.0:2183)(secure=disabled):Follower@172] - Got zxid 0x200000001 expected 0x1
    2021-11-30 15:01:18,225 [myid:3] - INFO  [CommitProcessor:3:LearnerSessionTracker@116] - Committing global session 0x5001d7cc6100000
    2021-11-30 15:01:18,898 [myid:3] - INFO  [CommitProcessor:3:LearnerSessionTracker@116] - Committing global session 0x3001d7cc6020000
    2021-11-30 15:01:19,304 [myid:3] - WARN  [NIOWorkerThread-6:NIOServerCnxn@371] - Unexpected exception
    EndOfStreamException: Unable to read additional data from client, it probably closed the socket: address = /127.0.0.1:55394, session = 0x3001d7cc6020000
            at org.apache.zookeeper.server.NIOServerCnxn.handleFailedRead(NIOServerCnxn.java:170)
            at org.apache.zookeeper.server.NIOServerCnxn.doIO(NIOServerCnxn.java:333)
            at org.apache.zookeeper.server.NIOServerCnxnFactory$IOWorkRequest.doWork(NIOServerCnxnFactory.java:508)
            at org.apache.zookeeper.server.WorkerService$ScheduledWorkRequest.run(WorkerService.java:154)
            at java.base/java.util.concurrent.ThreadPoolExecutor.runWorker(ThreadPoolExecutor.java:1128)
            at java.base/java.util.concurrent.ThreadPoolExecutor$Worker.run(ThreadPoolExecutor.java:628)
            at java.base/java.lang.Thread.run(Thread.java:829)
    2021-11-30 15:01:19,560 [myid:3] - INFO  [CommitProcessor:3:LearnerSessionTracker@116] - Committing global session 0x5001d7cc6100001
    2021-11-30 15:01:20,192 [myid:3] - INFO  [CommitProcessor:3:LearnerSessionTracker@116] - Committing global session 0x1001d7cc5e20000
    2021-11-30 15:01:20,828 [myid:3] - INFO  [CommitProcessor:3:LearnerSessionTracker@116] - Committing global session 0x5001d7cc6100002
    ```

    issues
    * https://issues.apache.org/jira/browse/ZOOKEEPER-4410?jql=text%20~%20%22Unable%20to%20read%20additional%20data%20from%20client%22

### NullPointerException in Learner.java

    ```
    2024-07-04 19:38:49,333 [myid:3] - WARN  [QuorumPeer[myid=3](plain=[0:0:0:0:0:0:0:0]:2183)(secure=disabled):Follower@131] - Exception when following the leader
    java.lang.NullPointerException
            at org.apache.zookeeper.server.quorum.Learner.syncWithLeader(Learner.java:652)
            at org.apache.zookeeper.server.quorum.Follower.followLeader(Follower.java:109)
            at org.apache.zookeeper.server.quorum.QuorumPeer.run(QuorumPeer.java:1514)
    ```

### NullPointerException in ServerCnxnFactory.java

    ```
    2024-01-08 12:20:56,590 [myid:2] - WARN  [CommitProcessor:2:ZooKeeperServer@1046] - Failed to register with JMX
    java.lang.NullPointerException
            at org.apache.zookeeper.server.ServerCnxnFactory.registerConnection(ServerCnxnFactory.java:217)
            at org.apache.zookeeper.server.ZooKeeperServer.finishSessionInit(ZooKeeperServer.java:1040)
            at org.apache.zookeeper.server.FinalRequestProcessor.processRequest(FinalRequestProcessor.java:226)
            at org.apache.zookeeper.server.quorum.CommitProcessor.processWrite(CommitProcessor.java:477)
            at org.apache.zookeeper.server.quorum.CommitProcessor.run(CommitProcessor.java:381)
    ```