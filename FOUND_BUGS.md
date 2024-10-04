# Found bugs by DistFuzz

<style>
table th:first-of-type {
    width: 10%;
}
table th:nth-of-type(2) {
    width: 10%;
}
table th:nth-of-type(3) {
    width: 50%;
}
table th:nth-of-type(4) {
    width: 30%;
}
</style>

| System     | ID           | Description                                                  | URL                                                                                         | Checker      | Confirmed? | Fixed? |
| ---------- | ------------ | ------------------------------------------------------------ | ------------------------------------------------------------------------------------------- | ------------ | ---------- | ------ |
| Braft      | Braft-1      | Memory leak                                                  | [braft-268](https://github.com/baidu/braft/issues/268)                                      | ASan         | Yes        |        |
| Braft      | Braft-2      | Fail to rename                                               | [braft-272](https://github.com/baidu/braft/issues/272)                                      | Log          | Yes        | Yes    |
| Braft      | Braft-3      | Raft error                                                   | [braft-273](https://github.com/baidu/braft/issues/273)                                      | Log          |            |        |
| Braft      | Braft-4      | Grpc error                                                   | [grpc-20418](https://github.com/grpc/grpc/issues/20418)                                     | Availability | Yes        |        |
| Braft      | Braft-5      | Assertion fail                                               | [braft-279](https://github.com/baidu/braft/issues/279)                                      | Crash        |            |        |
| Braft      | Braft-6      | Assertion fail                                               | [braft-280](https://github.com/baidu/braft/issues/280)                                      | Crash        |            |        |
| Braft      | Braft-7      | too many \`reject term_unmatched appendentries\`             | [braft-330](https://github.com/baidu/braft/issues/330)                                      | Availability | Yes        |        |
| Braft      | Braft-8      | Client failed                                                | [braft-338](https://github.com/baidu/braft/issues/338)                                      | Availability | Yes        |        |
| Braft      | Braft-9      | Brpc asan                                                    | [brpc-1624](https://github.com/apache/brpc/issues/1624)                                     | ASan         |            |        |
| Braft      | Braft-10     | Assertion fail                                               | [braft-report-segv](braft_test/report.md#segv)                                              | Log          |            |        |
| Dqlite     | Dqlite-1     | Failed to start after killing                                | [dqlite-199](https://github.com/canonical/raft/issues/199)                                  | Crash        | Yes        | Yes    |
| Dqlite     | Dqlite-2     | AddressSanitizer:DEADLYSIGNAL                                | [dqlite-report-deadsignal](c-raft_test/report.md#addresssanitizerdeadlysignal)              | ASan         |            |        |
| Dqlite     | Dqlite-3     | AddressSanitizer: bad-free                                   | [dqlite-219](https://github.com/canonical/raft/issues/219)                                  | ASan         | Yes        | Yes    |
| Dqlite     | Dqlite-4     | Heap-buffer-overflow                                         | [dqlite-report-buffer-overflow](c-raft_test/report.md#heap-buffer-overflow)                 | ASan         |            |        |
| Dqlite     | Dqlite-5     | Memory leak in uvSnapshotLoadMeta                            | [dqlite-249](https://github.com/canonical/raft/pull/249)                                    | ASan         | Yes        | Yes    |
| Dqlite     | Dqlite-6     | Assertion fail                                               | [dqlite-355](https://github.com/canonical/raft/issues/355)                                  | Crash        | Yes        | Yes    |
| Dqlite     | Dqlite-7     | Assertion fail                                               | [dqlite-386](https://github.com/canonical/raft/issues/386)                                  | Crash        | Yes        |        |
| Dqlite     | Dqlite-8     | Assertion fail                                               | [dqlite-assertion3](c-raft_test/report.md#assertion-request-argsn_entries--n)               | Crash        |            |        |
| Dqlite     | Dqlite-9     | Assertion fail                                               | [dqlite-assertion4](c-raft_test/report.md#assertion-t-readlen--0-failed)                    | Crash        |            |        |
| Dqlite     | Dqlite-10    | Heap-use-after-free                                          | [dqlite-669](https://github.com/canonical/dqlite/pull/669)                                  | Crash        | Yes        | Yes    |
| NuRaft     | NuRaft-1     | Crtl-D input issue for client                                | [nuraft-211](https://github.com/eBay/NuRaft/pull/211)                                       | Availability | Yes        | Yes    |
| NuRaft     | NuRaft-2     | FATL logs                                                    | [nuraft-271](https://github.com/eBay/NuRaft/issues/271)                                     | Crash        |            |        |
| Redis      | Redis-1      | Assertion fail                                               | [redisraft-104](https://github.com/RedisLabs/redisraft/issues/104)                          | Crash        | Yes        |        |
| Redis      | Redis-2      | REDIS RAFT PANIC: no such file                               | [redisraft-107](https://github.com/RedisLabs/redisraft/issues/107)                          | Crash        |            |        |
| Redis      | Redis-3      | Heap-buffer-overflow                                         | [redisraft-165](https://github.com/RedisLabs/redisraft/pull/165)                            | ASan         | Yes        | Yes    |
| Redis      | Redis-4      | Assertion fail                                               | [redisraft-report-assertion](redisraft_test/report.md#assertion-failed)                     | Crash        |            |        |
| RethinkDB  | RethinkDB-1  | Stack-use-after-scope                                        | [rethinkdb-6956](https://github.com/rethinkdb/rethinkdb/issues/6956)                        | ASan         | Yes        |        |
| RethinkDB  | RethinkDB-2  | Memory leak                                                  | [rethinkdb-6956](https://github.com/rethinkdb/rethinkdb/issues/6956)                        | ASan         | Yes        |        |
| RethinkDB  | RethinkDB-3  | Stack-use-after-scope                                        | [rethinkdb-report-uas1](rethinkdb_test/report.md#stack-use-after-scope-1)                   | ASan         |            |        |
| RethinkDB  | RethinkDB-4  | Table is ambiguous.                                          | [rethinkdb-4521](https://github.com/rethinkdb/rethinkdb/issues/4521)                        | Availability | Yes        |        |
| RethinkDB  | RethinkDB-5  | Assertion fail                                               | [rethinkdb-6961](https://github.com/rethinkdb/rethinkdb/issues/6961)                        | Crash        | Yes        |        |
| RethinkDB  | RethinkDB-6  | Assertion fail                                               | [rethinkdb-report-guarantee2](rethinkdb_test/report.md#grarantee-failed)                    | Crash        |            |
| RethinkDB  | RethinkDB-7  | Assertion fail                                               | [rethinkdb-report-guarantee3](rethinkdb_test/report.md#guarantee-failed-it-second--nullptr) | Log          |            |        |
| RethinkDB  | RethinkDB-8  | Assertion fail                                               | [rethinkdb-report-guarantee4](rethinkdb_test/report.md#guarantee-failed-peeripssize--0)     | Log          |            |        |
| RethinkDB  | RethinkDB-9  | Stack overflow                                               | [rethinkdb-report-stack-overflow](rethinkdb_test/report.md#stack-overflow)                  | ASan         |            |        |
| Aerospike  | Aerospike-1  | CRITICAL log in fabric                                       | [aerospike-server-27](https://github.com/aerospike/aerospike-server/issues/27)              | Crash        | Yes        |        |
| Aerospike  | Aerospike-2  | Stale read                                                   | [aerospike-server-30](https://github.com/aerospike/aerospike-server/issues/30)              | Linear       | Yes        |        |
| Aerospike  | Aerospike-3  | Stack-buffer-underflow                                       | [aerospike-server-33](https://github.com/aerospike/aerospike-server/issues/33)              | ASan         | Yes        |        |
| ClickHouse | ClickHouse-1 | Logic error                                                  | [clickhouse-26015](https://github.com/ClickHouse/ClickHouse/issues/26015)                   | Crash        | Yes        |        |
| ClickHouse | ClickHouse-2 | Create db fatal log                                          | [clickhouse-29255](https://github.com/ClickHouse/ClickHouse/issues/29255)                   | Crash        |            |        |
| etcd       | etcd-1       | Slice bounds out of range                                    | [etcd-13493](https://github.com/etcd-io/etcd/issues/13493)                                  | Crash        |            |        |
| etcd       | etcd-2       | Etcd-3517                                                    | [etcd-3517](https://github.com/etcd-io/etcd/issues/3517)                                    | Availability | Yes        | Yes    |
| ectd       | etcd-3       | Tocommit() is out of range                                   | [etcd-13493](https://github.com/etcd-io/etcd/issues/13493)                                  | Crash        | Yes        | Yes    |
| ZooKeeper  | ZooKeeper-1  | The accepted epoch, 9 is less than the current epoch         | [zookeeper-2307](https://issues.apache.org/jira/browse/ZOOKEEPER-2307)                      | Crash        | Yes        | Yes    |
| ZooKeeper  | ZooKeeper-2  | Severe unrecoverable error                                   | [zookeeper-4408](https://issues.apache.org/jira/browse/ZOOKEEPER-4408)                      | Crash        |            |        |
| ZooKeeper  | ZooKeeper-3  | NullPointerException in SendAckRequestProcessor              | [zookeeper-4409](https://issues.apache.org/jira/browse/ZOOKEEPER-4409)                      | Crash        | Yes        | Yes    |
| ZooKeeper  | ZooKeeper-4  | Committing zxid 0x100000003 but next pending txn 0x100000002 | [zookeeper-4418](https://issues.apache.org/jira/browse/ZOOKEEPER-4418)                      | Crash        |            |        |
| ZooKeeper  | ZooKeeper-5  | ZooKeeperServer not running                                  | [zookeeper-1770](https://github.com/apache/zookeeper/pull/1770)                             | Availability | Yes        | Yes    |
| ZooKeeper  | ZooKeeper-6  | Client fail                                                  | [zookeeper-4410](https://issues.apache.org/jira/browse/ZOOKEEPER-4410)                      | Availability | Yes        |        |
| ZooKeeper  | ZooKeeper-7  | ZK-2212                                                      | [zookeeper-2212](https://issues.apache.org/jira/browse/ZOOKEEPER-2212)                      | Availability | Yes        | Yes    |
| ZooKeeper  | ZooKeeper-8  | NullPointerException in Learner.java                         | [zookeeper-4394](https://github.com/apache/zookeeper/pull/2152)                             | Crash        | Yes        | Yes    |
| HDFS       | HDFS-50      | Safemode bug                                                 | [HDFS-16508](https://issues.apache.org/jira/browse/HDFS-16508)                              | Log          | Yes        |        |
| HDFS       | HDFS-51      | NPE in ipc/Client.java                                       | [HDFS-17562](https://issues.apache.org/jira/browse/HDFS-17562)                              | Crash        |            |        |
| HDFS       | HDFS-52      | IPC's epoch 1 is not the current writer epoch                | [HDFS-17563](https://issues.apache.org/jira/browse/HDFS-17563)                              | Crash        |            |        |
