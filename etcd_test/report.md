1. slice bounds out of range
    issues:
    * https://github.com/etcd-io/etcd/issues/13493

2. etcd-3517

3. tocommit() is out of range

    ```
    {"level":"info","ts":"2024-01-07T19:15:24.003+0100","caller":"embed/serve.go:98","msg":"ready to serve client requests"}
    {"level":"info","ts":"2024-01-07T19:15:24.004+0100","caller":"etcdmain/main.go:47","msg":"notifying init daemon"}
    {"level":"info","ts":"2024-01-07T19:15:24.004+0100","caller":"etcdmain/main.go:53","msg":"successfully notified init daemon"}
    {"level":"info","ts":"2024-01-07T19:15:24.004+0100","caller":"embed/serve.go:140","msg":"serving client traffic insecurely; this is strongly discouraged!","address":"127.0.1.1:2482"}
    {"level":"panic","ts":"2024-01-07T19:15:24.102+0100","logger":"raft","caller":"etcdserver/zap_raft.go:101","msg":"tocommit(11) is out of range [lastIndex(10)]. Was the raft log corrupted, truncated, or lost?","stacktrace":"go.etcd.io/etcd/server/v3/etcdserver.(*zapRaftLogger).Panicf\n\t/tmp/etcd-release-3.5.1/etcd/release/etcd/server/etcdserver/zap_raft.go:101\ngo.etcd.io/etcd/raft/v3.(*raftLog).commitTo\n\t/tmp/etcd-release-3.5.1/etcd/release/etcd/raft/log.go:237\ngo.etcd.io/etcd/raft/v3.(*raft).handleHeartbeat\n\t/tmp/etcd-release-3.5.1/etcd/release/etcd/raft/raft.go:1508\ngo.etcd.io/etcd/raft/v3.stepFollower\n\t/tmp/etcd-release-3.5.1/etcd/release/etcd/raft/raft.go:1434\ngo.etcd.io/etcd/raft/v3.(*raft).Step\n\t/tmp/etcd-release-3.5.1/etcd/release/etcd/raft/raft.go:975\ngo.etcd.io/etcd/raft/v3.(*node).run\n\t/tmp/etcd-release-3.5.1/etcd/release/etcd/raft/node.go:356"}
    panic: tocommit(11) is out of range [lastIndex(10)]. Was the raft log corrupted, truncated, or lost?

    goroutine 175 [running]:
    go.uber.org/zap/zapcore.(*CheckedEntry).Write(0xc00031a240, 0x0, 0x0, 0x0)
            /home/remote/sbatsche/.gvm/pkgsets/go1.16.3/global/pkg/mod/go.uber.org/zap@v1.17.0/zapcore/entry.go:234 +0x58d
    go.uber.org/zap.(*SugaredLogger).log(0xc00000e3a0, 0x4, 0x12498a4, 0x5d, 0xc00014d2c0, 0x2, 0x2, 0x0, 0x0, 0x0)
            /home/remote/sbatsche/.gvm/pkgsets/go1.16.3/global/pkg/mod/go.uber.org/zap@v1.17.0/sugar.go:227 +0x111
    go.uber.org/zap.(*SugaredLogger).Panicf(...)
            /home/remote/sbatsche/.gvm/pkgsets/go1.16.3/global/pkg/mod/go.uber.org/zap@v1.17.0/sugar.go:159
    go.etcd.io/etcd/server/v3/etcdserver.(*zapRaftLogger).Panicf(0xc0004881c0, 0x12498a4, 0x5d, 0xc00014d2c0, 0x2, 0x2)
            /tmp/etcd-release-3.5.1/etcd/release/etcd/server/etcdserver/zap_raft.go:101 +0x7d
    go.etcd.io/etcd/raft/v3.(*raftLog).commitTo(0xc000148000, 0xb)
            /tmp/etcd-release-3.5.1/etcd/release/etcd/raft/log.go:237 +0x135
    go.etcd.io/etcd/raft/v3.(*raft).handleHeartbeat(0xc000523080, 0x8, 0xed08be6981de8fb2, 0xa5227d6d18885972, 0x2, 0x0, 0x0, 0x0, 0x0, 0x0, ...)
            /tmp/etcd-release-3.5.1/etcd/release/etcd/raft/raft.go:1508 +0x54
    go.etcd.io/etcd/raft/v3.stepFollower(0xc000523080, 0x8, 0xed08be6981de8fb2, 0xa5227d6d18885972, 0x2, 0x0, 0x0, 0x0, 0x0, 0x0, ...)
            /tmp/etcd-release-3.5.1/etcd/release/etcd/raft/raft.go:1434 +0x478
    go.etcd.io/etcd/raft/v3.(*raft).Step(0xc000523080, 0x8, 0xed08be6981de8fb2, 0xa5227d6d18885972, 0x2, 0x0, 0x0, 0x0, 0x0, 0x0, ...)
            /tmp/etcd-release-3.5.1/etcd/release/etcd/raft/raft.go:975 +0xa55
    go.etcd.io/etcd/raft/v3.(*node).run(0xc0004b62a0)
            /tmp/etcd-release-3.5.1/etcd/release/etcd/raft/node.go:356 +0x798
    created by go.etcd.io/etcd/raft/v3.StartNode
            /tmp/etcd-release-3.5.1/etcd/release/etcd/raft/node.go:230 +0x375
    dex: 5] sent MsgPreVote request to 48ea5f4fca1499b at term 1"}
    {"level":"info","ts":"2024-01-07T19:13:36.150+0100","logger":"raft","caller":"etcdserver/zap_raft.go:77","msg":"ed08be6981de8fb2 [logterm: 1, index: 5] sent MsgPreVote request to a5227d6d18885972 at term 1"}
    {"level":"info","ts":"2024-01-07T19:13:36.150+0100","logger":"raft","caller":"etcdserver/zap_raft.go:77","msg":"ed08be6981de8fb2 [logterm: 1, index: 5] sent MsgPreVote request to a892a28439628834 at term 1"}
    {"level":"info","ts":"2024-01-07T19:13:36.150+0100","logger":"raft","caller":"etcdserver/zap_raft.go:77","msg":"ed08be6981de8fb2 [logterm: 1, index: 5] sent MsgPreVote request to e9b32fefc3f8c519 at term 1"}
    ```