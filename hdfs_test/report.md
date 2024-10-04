### safemode bug

    This is a trivial bug that can be found by any tools actually

    https://issues.apache.org/jira/browse/HDFS-16508

### NullPointerException

    ```
    2024-06-28 03:07:58,649 WARN org.apache.hadoop.hdfs.server.datanode.DataNode: IOException in offerService
    java.io.IOException: DestHost:destPort localhost:9000 , LocalHost:localPort e07ff098d9e2/172.17.0.4:0. Failed on local exception: java.io.IOException: Error reading responses
            at java.base/jdk.internal.reflect.NativeConstructorAccessorImpl.newInstance0(Native Method)
            at java.base/jdk.internal.reflect.NativeConstructorAccessorImpl.newInstance(NativeConstructorAccessorImpl.java:62)
            at java.base/jdk.internal.reflect.DelegatingConstructorAccessorImpl.newInstance(DelegatingConstructorAccessorImpl.java:45)
            at java.base/java.lang.reflect.Constructor.newInstance(Constructor.java:490)
            at org.apache.hadoop.net.NetUtils.wrapWithMessage(NetUtils.java:842)
            at org.apache.hadoop.net.NetUtils.wrapException(NetUtils.java:817)
            at org.apache.hadoop.ipc.Client.getRpcResponse(Client.java:1616)
            at org.apache.hadoop.ipc.Client.call(Client.java:1558)
            at org.apache.hadoop.ipc.Client.call(Client.java:1455)
            at org.apache.hadoop.ipc.ProtobufRpcEngine$Invoker.invoke(ProtobufRpcEngine.java:231)
            at org.apache.hadoop.ipc.ProtobufRpcEngine$Invoker.invoke(ProtobufRpcEngine.java:118)
            at com.sun.proxy.$Proxy19.sendHeartbeat(Unknown Source)
            at org.apache.hadoop.hdfs.protocolPB.DatanodeProtocolClientSideTranslatorPB.sendHeartbeat(DatanodeProtocolClientSideTranslatorPB.java:168)
            at org.apache.hadoop.hdfs.server.datanode.BPServiceActor.sendHeartBeat(BPServiceActor.java:524)
            at org.apache.hadoop.hdfs.server.datanode.BPServiceActor.offerService(BPServiceActor.java:658)
            at org.apache.hadoop.hdfs.server.datanode.BPServiceActor.run(BPServiceActor.java:855)
            at java.base/java.lang.Thread.run(Thread.java:829)
    Caused by: java.io.IOException: Error reading responses
            at org.apache.hadoop.ipc.Client$Connection.run(Client.java:1141)
    Caused by: java.lang.NullPointerException
            at org.apache.hadoop.ipc.Client$Connection.receiveRpcResponse(Client.java:1252)
            at org.apache.hadoop.ipc.Client$Connection.run(Client.java:1134)
    ```

### IPC's epoch 1 is not the current writer epoch

    ```
    2024-06-28 03:07:57,334 WARN org.apache.hadoop.hdfs.qjournal.client.QuorumJournalManager: Remote journal 127.0.0.1:8485 failed to write txns 4-5. Will try to write to this JN again after the next log roll.
    org.apache.hadoop.ipc.RemoteException(java.io.IOException): IPC's epoch 1 is not the current writer epoch  0 ; journal id: mycluster
            at org.apache.hadoop.hdfs.qjournal.server.Journal.checkWriteRequest(Journal.java:521)
            at org.apache.hadoop.hdfs.qjournal.server.Journal.journal(Journal.java:398)
            at org.apache.hadoop.hdfs.qjournal.server.JournalNodeRpcServer.journal(JournalNodeRpcServer.java:191)
            at org.apache.hadoop.hdfs.qjournal.protocolPB.QJournalProtocolServerSideTranslatorPB.journal(QJournalProtocolServerSideTranslatorPB.java:164)
            at org.apache.hadoop.hdfs.qjournal.protocol.QJournalProtocolProtos$QJournalProtocolService$2.callBlockingMethod(QJournalProtocolProtos.java:28974)
            at org.apache.hadoop.ipc.ProtobufRpcEngine$Server$ProtoBufRpcInvoker.call(ProtobufRpcEngine.java:549)
            at org.apache.hadoop.ipc.ProtobufRpcEngine$Server$ProtoBufRpcInvoker.call(ProtobufRpcEngine.java:518)
            at org.apache.hadoop.ipc.RPC$Server.call(RPC.java:1086)
            at org.apache.hadoop.ipc.Server$RpcCall.run(Server.java:1035)
            at org.apache.hadoop.ipc.Server$RpcCall.run(Server.java:963)
            at java.base/java.security.AccessController.doPrivileged(Native Method)
            at java.base/javax.security.auth.Subject.doAs(Subject.java:423)
            at org.apache.hadoop.security.UserGroupInformation.doAs(UserGroupInformation.java:1762)
            at org.apache.hadoop.ipc.Server$Handler.run(Server.java:2960)

            at org.apache.hadoop.ipc.Client.getRpcResponse(Client.java:1612)
            at org.apache.hadoop.ipc.Client.call(Client.java:1558)
            at org.apache.hadoop.ipc.Client.call(Client.java:1455)
            at org.apache.hadoop.ipc.ProtobufRpcEngine$Invoker.invoke(ProtobufRpcEngine.java:231)
            at org.apache.hadoop.ipc.ProtobufRpcEngine$Invoker.invoke(ProtobufRpcEngine.java:118)
            at com.sun.proxy.$Proxy14.journal(Unknown Source)
            at org.apache.hadoop.hdfs.qjournal.protocolPB.QJournalProtocolTranslatorPB.journal(QJournalProtocolTranslatorPB.java:191)
            at org.apache.hadoop.hdfs.qjournal.client.IPCLoggerChannel$7.call(IPCLoggerChannel.java:401)
            at org.apache.hadoop.hdfs.qjournal.client.IPCLoggerChannel$7.call(IPCLoggerChannel.java:394)
            at com.google.common.util.concurrent.TrustedListenableFutureTask$TrustedFutureInterruptibleTask.runInterruptibly(TrustedListenableFutureTask.java:125)
            at com.google.common.util.concurrent.InterruptibleTask.run(InterruptibleTask.java:57)
            at com.google.common.util.concurrent.TrustedListenableFutureTask.run(TrustedListenableFutureTask.java:78)
            at java.base/java.util.concurrent.ThreadPoolExecutor.runWorker(ThreadPoolExecutor.java:1128)
            at java.base/java.util.concurrent.ThreadPoolExecutor$Worker.run(ThreadPoolExecutor.java:628)
            at java.base/java.lang.Thread.run(Thread.java:829)
    ```