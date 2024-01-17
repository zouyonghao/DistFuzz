1. slice bounds out of range
    issues:
    * https://github.com/etcd-io/etcd/issues/13493

    ```
        [[FUZZ PRINT]Get Environment Variable 3]
        [You are running in a fuzzing environment]
        {"level":"info","ts":"2024-01-07T18:21:45.750+0100","caller":"etcdmain/etcd.go:72","msg":"Running: ","args":["/home/zyh/etcd/bin/etcd","--name","node2","--data-dir","./data2","--heartbeat-interval","100","--election-timeout","500","--socket-reuse-port","--socket-reuse-address","--initial-cluster-token","etcd-cluster","--initial-cluster-state","new","--initial-advertise-peer-urls","http://127.0.1.1:2382","--listen-peer-urls","http://127.0.1.1:2382","--listen-client-urls","http://127.0.1.1:2482","--advertise-client-urls","http://127.0.1.1:2482","--initial-cluster","node0=http://127.0.1.1:2380,node1=http://127.0.1.1:2381,node2=http://127.0.1.1:2382,node3=http://127.0.1.1:2383,node4=http://127.0.1.1:2384"]}
        {"level":"info","ts":"2024-01-07T18:21:45.750+0100","caller":"etcdmain/etcd.go:99","msg":"failed to detect default host","error":"could not find default route"}
        {"level":"info","ts":"2024-01-07T18:21:45.750+0100","caller":"embed/etcd.go:125","msg":"configuring socket options","reuse-address":true,"reuse-port":true}
        {"level":"info","ts":"2024-01-07T18:21:45.750+0100","caller":"embed/etcd.go:131","msg":"configuring peer listeners","listen-peer-urls":["http://127.0.1.1:2382"]}
        {"level":"info","ts":"2024-01-07T18:21:45.751+0100","caller":"embed/etcd.go:139","msg":"configuring client listeners","listen-client-urls":["http://127.0.1.1:2482"]}
        {"level":"info","ts":"2024-01-07T18:21:45.752+0100","caller":"embed/etcd.go:307","msg":"starting an etcd server","etcd-version":"3.5.1","git-sha":"e8732fb5f","go-version":"go1.16.3","go-os":"linux","go-arch":"amd64","max-cpu-set":48,"max-cpu-available":48,"member-initialized":false,"name":"node2","data-dir":"./data2","wal-dir":"","wal-dir-dedicated":"","member-dir":"data2/member","force-new-cluster":false,"heartbeat-interval":"100ms","election-timeout":"500ms","initial-election-tick-advance":true,"snapshot-count":100000,"snapshot-catchup-entries":5000,"initial-advertise-peer-urls":["http://127.0.1.1:2382"],"listen-peer-urls":["http://127.0.1.1:2382"],"advertise-client-urls":["http://127.0.1.1:2482"],"listen-client-urls":["http://127.0.1.1:2482"],"listen-metrics-urls":[],"cors":["*"],"host-whitelist":["*"],"initial-cluster":"node0=http://127.0.1.1:2380,node1=http://127.0.1.1:2381,node2=http://127.0.1.1:2382,node3=http://127.0.1.1:2383,node4=http://127.0.1.1:2384","initial-cluster-state":"new","initial-cluster-token":"etcd-cluster","quota-size-bytes":2147483648,"pre-vote":true,"initial-corrupt-check":false,"corrupt-check-time-interval":"0s","auto-compaction-mode":"periodic","auto-compaction-retention":"0s","auto-compaction-interval":"0s","discovery-url":"","discovery-proxy":"","downgrade-check-interval":"5s"}
        {"level":"info","ts":"2024-01-07T18:21:45.755+0100","caller":"etcdserver/backend.go:81","msg":"opened backend db","path":"data2/member/snap/db","took":"1.207523ms"}
        {"level":"info","ts":"2024-01-07T18:21:45.788+0100","caller":"etcdserver/raft.go:448","msg":"starting local member","local-member-id":"ed08be6981de8fb2","cluster-id":"60494380bb9bd798"}
        {"level":"info","ts":"2024-01-07T18:21:45.788+0100","logger":"raft","caller":"etcdserver/zap_raft.go:77","msg":"ed08be6981de8fb2 switched to configuration voters=()"}
        {"level":"info","ts":"2024-01-07T18:21:45.788+0100","logger":"raft","caller":"etcdserver/zap_raft.go:77","msg":"ed08be6981de8fb2 became follower at term 0"}
        {"level":"info","ts":"2024-01-07T18:21:45.788+0100","logger":"raft","caller":"etcdserver/zap_raft.go:77","msg":"newRaft ed08be6981de8fb2 [peers: [], term: 0, commit: 0, applied: 0, lastindex: 0, lastterm: 0]"}
        {"level":"info","ts":"2024-01-07T18:21:45.788+0100","logger":"raft","caller":"etcdserver/zap_raft.go:77","msg":"ed08be6981de8fb2 became follower at term 1"}
        {"level":"info","ts":"2024-01-07T18:21:45.788+0100","logger":"raft","caller":"etcdserver/zap_raft.go:77","msg":"ed08be6981de8fb2 switched to configuration voters=(328382294473656731)"}
        {"level":"info","ts":"2024-01-07T18:21:45.788+0100","logger":"raft","caller":"etcdserver/zap_raft.go:77","msg":"ed08be6981de8fb2 switched to configuration voters=(328382294473656731 11899211072982767986)"}
        {"level":"info","ts":"2024-01-07T18:21:45.789+0100","logger":"raft","caller":"etcdserver/zap_raft.go:77","msg":"ed08be6981de8fb2 switched to configuration voters=(328382294473656731 11899211072982767986 12146949833753790516)"}
        {"level":"info","ts":"2024-01-07T18:21:45.789+0100","logger":"raft","caller":"etcdserver/zap_raft.go:77","msg":"ed08be6981de8fb2 switched to configuration voters=(328382294473656731 11899211072982767986 12146949833753790516 16839856138499966233)"}
        {"level":"info","ts":"2024-01-07T18:21:45.789+0100","logger":"raft","caller":"etcdserver/zap_raft.go:77","msg":"ed08be6981de8fb2 switched to configuration voters=(328382294473656731 11899211072982767986 12146949833753790516 16839856138499966233 17080110947162296242)"}
        {"level":"warn","ts":"2024-01-07T18:21:45.791+0100","caller":"auth/store.go:1220","msg":"simple token is not cryptographically signed"}
        {"level":"info","ts":"2024-01-07T18:21:45.792+0100","caller":"mvcc/kvstore.go:415","msg":"kvstore restored","current-rev":1}
        {"level":"info","ts":"2024-01-07T18:21:45.793+0100","caller":"etcdserver/quota.go:94","msg":"enabled backend quota with default value","quota-name":"v3-applier","quota-size-bytes":2147483648,"quota-size":"2.1 GB"}
        {"level":"info","ts":"2024-01-07T18:21:45.794+0100","caller":"rafthttp/peer.go:133","msg":"starting remote peer","remote-peer-id":"48ea5f4fca1499b"}
        {"level":"info","ts":"2024-01-07T18:21:45.794+0100","caller":"rafthttp/pipeline.go:72","msg":"started HTTP pipelining with remote peer","local-member-id":"ed08be6981de8fb2","remote-peer-id":"48ea5f4fca1499b"}
        {"level":"info","ts":"2024-01-07T18:21:45.796+0100","caller":"rafthttp/stream.go:169","msg":"started stream writer with remote peer","local-member-id":"ed08be6981de8fb2","remote-peer-id":"48ea5f4fca1499b"}
        {"level":"info","ts":"2024-01-07T18:21:45.797+0100","caller":"rafthttp/stream.go:169","msg":"started stream writer with remote peer","local-member-id":"ed08be6981de8fb2","remote-peer-id":"48ea5f4fca1499b"}
        {"level":"info","ts":"2024-01-07T18:21:45.800+0100","caller":"rafthttp/peer.go:137","msg":"started remote peer","remote-peer-id":"48ea5f4fca1499b"}
        {"level":"info","ts":"2024-01-07T18:21:45.800+0100","caller":"rafthttp/transport.go:317","msg":"added remote peer","local-member-id":"ed08be6981de8fb2","remote-peer-id":"48ea5f4fca1499b","remote-peer-urls":["http://127.0.1.1:2384"]}
        {"level":"info","ts":"2024-01-07T18:21:45.800+0100","caller":"rafthttp/stream.go:395","msg":"started stream reader with remote peer","stream-reader-type":"stream Message","local-member-id":"ed08be6981de8fb2","remote-peer-id":"48ea5f4fca1499b"}
        {"level":"info","ts":"2024-01-07T18:21:45.800+0100","caller":"rafthttp/peer.go:133","msg":"starting remote peer","remote-peer-id":"a5227d6d18885972"}
        {"level":"info","ts":"2024-01-07T18:21:45.800+0100","caller":"rafthttp/pipeline.go:72","msg":"started HTTP pipelining with remote peer","local-member-id":"ed08be6981de8fb2","remote-peer-id":"a5227d6d18885972"}
        {"level":"info","ts":"2024-01-07T18:21:45.800+0100","caller":"rafthttp/stream.go:395","msg":"started stream reader with remote peer","stream-reader-type":"stream MsgApp v2","local-member-id":"ed08be6981de8fb2","remote-peer-id":"48ea5f4fca1499b"}
        {"level":"info","ts":"2024-01-07T18:21:45.801+0100","caller":"rafthttp/stream.go:169","msg":"started stream writer with remote peer","local-member-id":"ed08be6981de8fb2","remote-peer-id":"a5227d6d18885972"}
        {"level":"info","ts":"2024-01-07T18:21:45.803+0100","caller":"rafthttp/stream.go:169","msg":"started stream writer with remote peer","local-member-id":"ed08be6981de8fb2","remote-peer-id":"a5227d6d18885972"}
        {"level":"info","ts":"2024-01-07T18:21:45.804+0100","caller":"rafthttp/peer.go:137","msg":"started remote peer","remote-peer-id":"a5227d6d18885972"}
        {"level":"info","ts":"2024-01-07T18:21:45.804+0100","caller":"rafthttp/transport.go:317","msg":"added remote peer","local-member-id":"ed08be6981de8fb2","remote-peer-id":"a5227d6d18885972","remote-peer-urls":["http://127.0.1.1:2380"]}
        {"level":"info","ts":"2024-01-07T18:21:45.804+0100","caller":"rafthttp/peer.go:133","msg":"starting remote peer","remote-peer-id":"a892a28439628834"}
        {"level":"info","ts":"2024-01-07T18:21:45.804+0100","caller":"rafthttp/pipeline.go:72","msg":"started HTTP pipelining with remote peer","local-member-id":"ed08be6981de8fb2","remote-peer-id":"a892a28439628834"}
        {"level":"info","ts":"2024-01-07T18:21:45.804+0100","caller":"rafthttp/stream.go:395","msg":"started stream reader with remote peer","stream-reader-type":"stream Message","local-member-id":"ed08be6981de8fb2","remote-peer-id":"a5227d6d18885972"}
        {"level":"info","ts":"2024-01-07T18:21:45.804+0100","caller":"rafthttp/stream.go:395","msg":"started stream reader with remote peer","stream-reader-type":"stream MsgApp v2","local-member-id":"ed08be6981de8fb2","remote-peer-id":"a5227d6d18885972"}
        {"level":"info","ts":"2024-01-07T18:21:45.812+0100","caller":"rafthttp/peer.go:137","msg":"started remote peer","remote-peer-id":"a892a28439628834"}
        {"level":"info","ts":"2024-01-07T18:21:45.814+0100","caller":"rafthttp/transport.go:317","msg":"added remote peer","local-member-id":"ed08be6981de8fb2","remote-peer-id":"a892a28439628834","remote-peer-urls":["http://127.0.1.1:2381"]}
        {"level":"info","ts":"2024-01-07T18:21:45.815+0100","caller":"rafthttp/stream.go:395","msg":"started stream reader with remote peer","stream-reader-type":"stream Message","local-member-id":"ed08be6981de8fb2","remote-peer-id":"a892a28439628834"}
        {"level":"info","ts":"2024-01-07T18:21:45.815+0100","caller":"rafthttp/peer.go:133","msg":"starting remote peer","remote-peer-id":"e9b32fefc3f8c519"}
        {"level":"info","ts":"2024-01-07T18:21:45.814+0100","caller":"rafthttp/stream.go:169","msg":"started stream writer with remote peer","local-member-id":"ed08be6981de8fb2","remote-peer-id":"a892a28439628834"}
        {"level":"info","ts":"2024-01-07T18:21:45.814+0100","caller":"rafthttp/stream.go:169","msg":"started stream writer with remote peer","local-member-id":"ed08be6981de8fb2","remote-peer-id":"a892a28439628834"}
        {"level":"info","ts":"2024-01-07T18:21:45.815+0100","caller":"rafthttp/stream.go:395","msg":"started stream reader with remote peer","stream-reader-type":"stream MsgApp v2","local-member-id":"ed08be6981de8fb2","remote-peer-id":"a892a28439628834"}
        {"level":"info","ts":"2024-01-07T18:21:45.815+0100","caller":"rafthttp/pipeline.go:72","msg":"started HTTP pipelining with remote peer","local-member-id":"ed08be6981de8fb2","remote-peer-id":"e9b32fefc3f8c519"}
        {"level":"info","ts":"2024-01-07T18:21:45.817+0100","caller":"rafthttp/peer.go:137","msg":"started remote peer","remote-peer-id":"e9b32fefc3f8c519"}
        {"level":"info","ts":"2024-01-07T18:21:45.818+0100","caller":"rafthttp/stream.go:169","msg":"started stream writer with remote peer","local-member-id":"ed08be6981de8fb2","remote-peer-id":"e9b32fefc3f8c519"}
        {"level":"info","ts":"2024-01-07T18:21:45.818+0100","caller":"rafthttp/transport.go:317","msg":"added remote peer","local-member-id":"ed08be6981de8fb2","remote-peer-id":"e9b32fefc3f8c519","remote-peer-urls":["http://127.0.1.1:2383"]}
        {"level":"info","ts":"2024-01-07T18:21:45.819+0100","caller":"etcdserver/server.go:843","msg":"starting etcd server","local-member-id":"ed08be6981de8fb2","local-server-version":"3.5.1","cluster-version":"to_be_decided"}
        {"level":"info","ts":"2024-01-07T18:21:45.819+0100","caller":"etcdserver/server.go:744","msg":"starting initial election tick advance","election-ticks":5}
        {"level":"info","ts":"2024-01-07T18:21:45.818+0100","caller":"rafthttp/stream.go:169","msg":"started stream writer with remote peer","local-member-id":"ed08be6981de8fb2","remote-peer-id":"e9b32fefc3f8c519"}
        {"level":"info","ts":"2024-01-07T18:21:45.818+0100","caller":"rafthttp/stream.go:395","msg":"started stream reader with remote peer","stream-reader-type":"stream MsgApp v2","local-member-id":"ed08be6981de8fb2","remote-peer-id":"e9b32fefc3f8c519"}
        {"level":"info","ts":"2024-01-07T18:21:45.818+0100","caller":"rafthttp/stream.go:395","msg":"started stream reader with remote peer","stream-reader-type":"stream Message","local-member-id":"ed08be6981de8fb2","remote-peer-id":"e9b32fefc3f8c519"}
        {"level":"info","ts":"2024-01-07T18:21:45.819+0100","logger":"raft","caller":"etcdserver/zap_raft.go:77","msg":"ed08be6981de8fb2 switched to configuration voters=(328382294473656731 11899211072982767986 12146949833753790516 16839856138499966233 17080110947162296242)"}
        {"level":"info","ts":"2024-01-07T18:21:45.820+0100","caller":"membership/cluster.go:421","msg":"added member","cluster-id":"60494380bb9bd798","local-member-id":"ed08be6981de8fb2","added-peer-id":"48ea5f4fca1499b","added-peer-peer-urls":["http://127.0.1.1:2384"]}
        {"level":"info","ts":"2024-01-07T18:21:45.820+0100","logger":"raft","caller":"etcdserver/zap_raft.go:77","msg":"ed08be6981de8fb2 switched to configuration voters=(328382294473656731 11899211072982767986 12146949833753790516 16839856138499966233 17080110947162296242)"}
        {"level":"info","ts":"2024-01-07T18:21:45.820+0100","caller":"embed/etcd.go:276","msg":"now serving peer/client/metrics","local-member-id":"ed08be6981de8fb2","initial-advertise-peer-urls":["http://127.0.1.1:2382"],"listen-peer-urls":["http://127.0.1.1:2382"],"advertise-client-urls":["http://127.0.1.1:2482"],"listen-client-urls":["http://127.0.1.1:2482"],"listen-metrics-urls":[]}
        {"level":"info","ts":"2024-01-07T18:21:45.820+0100","caller":"membership/cluster.go:421","msg":"added member","cluster-id":"60494380bb9bd798","local-member-id":"ed08be6981de8fb2","added-peer-id":"a5227d6d18885972","added-peer-peer-urls":["http://127.0.1.1:2380"]}
        {"level":"info","ts":"2024-01-07T18:21:45.820+0100","logger":"raft","caller":"etcdserver/zap_raft.go:77","msg":"ed08be6981de8fb2 switched to configuration voters=(328382294473656731 11899211072982767986 12146949833753790516 16839856138499966233 17080110947162296242)"}
        {"level":"info","ts":"2024-01-07T18:21:45.820+0100","caller":"embed/etcd.go:580","msg":"serving peer traffic","address":"127.0.1.1:2382"}
        {"level":"info","ts":"2024-01-07T18:21:45.825+0100","caller":"embed/etcd.go:552","msg":"cmux::serve","address":"127.0.1.1:2382"}
        {"level":"info","ts":"2024-01-07T18:21:45.822+0100","caller":"membership/cluster.go:421","msg":"added member","cluster-id":"60494380bb9bd798","local-member-id":"ed08be6981de8fb2","added-peer-id":"a892a28439628834","added-peer-peer-urls":["http://127.0.1.1:2381"]}
        {"level":"info","ts":"2024-01-07T18:21:45.822+0100","caller":"rafthttp/peer_status.go:53","msg":"peer became active","peer-id":"a5227d6d18885972"}
        {"level":"info","ts":"2024-01-07T18:21:45.827+0100","caller":"rafthttp/stream.go:412","msg":"established TCP streaming connection with remote peer","stream-reader-type":"stream Message","local-member-id":"ed08be6981de8fb2","remote-peer-id":"a5227d6d18885972"}
        {"level":"info","ts":"2024-01-07T18:21:45.827+0100","caller":"rafthttp/stream.go:412","msg":"established TCP streaming connection with remote peer","stream-reader-type":"stream MsgApp v2","local-member-id":"ed08be6981de8fb2","remote-peer-id":"a5227d6d18885972"}
        {"level":"info","ts":"2024-01-07T18:21:45.827+0100","logger":"raft","caller":"etcdserver/zap_raft.go:77","msg":"ed08be6981de8fb2 switched to configuration voters=(328382294473656731 11899211072982767986 12146949833753790516 16839856138499966233 17080110947162296242)"}
        {"level":"info","ts":"2024-01-07T18:21:45.829+0100","caller":"membership/cluster.go:421","msg":"added member","cluster-id":"60494380bb9bd798","local-member-id":"ed08be6981de8fb2","added-peer-id":"e9b32fefc3f8c519","added-peer-peer-urls":["http://127.0.1.1:2383"]}
        {"level":"info","ts":"2024-01-07T18:21:45.832+0100","logger":"raft","caller":"etcdserver/zap_raft.go:77","msg":"ed08be6981de8fb2 switched to configuration voters=(328382294473656731 11899211072982767986 12146949833753790516 16839856138499966233 17080110947162296242)"}
        {"level":"info","ts":"2024-01-07T18:21:45.834+0100","caller":"membership/cluster.go:421","msg":"added member","cluster-id":"60494380bb9bd798","local-member-id":"ed08be6981de8fb2","added-peer-id":"ed08be6981de8fb2","added-peer-peer-urls":["http://127.0.1.1:2382"]}
        {"level":"info","ts":"2024-01-07T18:21:45.841+0100","caller":"rafthttp/stream.go:249","msg":"set message encoder","from":"ed08be6981de8fb2","to":"a5227d6d18885972","stream-type":"stream MsgApp v2"}
        {"level":"info","ts":"2024-01-07T18:21:45.843+0100","caller":"rafthttp/stream.go:274","msg":"established TCP streaming connection with remote peer","stream-writer-type":"stream MsgApp v2","local-member-id":"ed08be6981de8fb2","remote-peer-id":"a5227d6d18885972"}
        {"level":"info","ts":"2024-01-07T18:21:45.843+0100","caller":"rafthttp/stream.go:249","msg":"set message encoder","from":"ed08be6981de8fb2","to":"a5227d6d18885972","stream-type":"stream Message"}
        {"level":"info","ts":"2024-01-07T18:21:45.846+0100","caller":"rafthttp/stream.go:274","msg":"established TCP streaming connection with remote peer","stream-writer-type":"stream Message","local-member-id":"ed08be6981de8fb2","remote-peer-id":"a5227d6d18885972"}
        {"level":"info","ts":"2024-01-07T18:21:45.846+0100","caller":"rafthttp/stream.go:249","msg":"set message encoder","from":"ed08be6981de8fb2","to":"e9b32fefc3f8c519","stream-type":"stream MsgApp v2"}
        {"level":"info","ts":"2024-01-07T18:21:45.847+0100","caller":"rafthttp/peer_status.go:53","msg":"peer became active","peer-id":"e9b32fefc3f8c519"}
        {"level":"info","ts":"2024-01-07T18:21:45.847+0100","caller":"rafthttp/stream.go:274","msg":"established TCP streaming connection with remote peer","stream-writer-type":"stream MsgApp v2","local-member-id":"ed08be6981de8fb2","remote-peer-id":"e9b32fefc3f8c519"}
        {"level":"info","ts":"2024-01-07T18:21:45.851+0100","caller":"rafthttp/stream.go:249","msg":"set message encoder","from":"ed08be6981de8fb2","to":"e9b32fefc3f8c519","stream-type":"stream Message"}
        {"level":"info","ts":"2024-01-07T18:21:45.852+0100","caller":"rafthttp/stream.go:274","msg":"established TCP streaming connection with remote peer","stream-writer-type":"stream Message","local-member-id":"ed08be6981de8fb2","remote-peer-id":"e9b32fefc3f8c519"}
        {"level":"info","ts":"2024-01-07T18:21:45.862+0100","caller":"rafthttp/stream.go:412","msg":"established TCP streaming connection with remote peer","stream-reader-type":"stream MsgApp v2","local-member-id":"ed08be6981de8fb2","remote-peer-id":"e9b32fefc3f8c519"}
        {"level":"info","ts":"2024-01-07T18:21:45.865+0100","caller":"rafthttp/stream.go:412","msg":"established TCP streaming connection with remote peer","stream-reader-type":"stream Message","local-member-id":"ed08be6981de8fb2","remote-peer-id":"e9b32fefc3f8c519"}
        {"level":"info","ts":"2024-01-07T18:21:45.869+0100","caller":"etcdserver/server.go:767","msg":"initialized peer connections; fast-forwarding election ticks","local-member-id":"ed08be6981de8fb2","forward-ticks":3,"forward-duration":"300ms","election-ticks":5,"election-timeout":"500ms","active-remote-members":2}
        {"level":"info","ts":"2024-01-07T18:21:46.064+0100","logger":"raft","caller":"etcdserver/zap_raft.go:77","msg":"ed08be6981de8fb2 [logterm: 1, index: 5, vote: 0] cast MsgPreVote for a5227d6d18885972 [logterm: 1, index: 5] at term 1"}
        {"level":"info","ts":"2024-01-07T18:21:46.078+0100","logger":"raft","caller":"etcdserver/zap_raft.go:77","msg":"ed08be6981de8fb2 [term: 1] received a MsgVote message with higher term from a5227d6d18885972 [term: 2]"}
        {"level":"info","ts":"2024-01-07T18:21:46.078+0100","logger":"raft","caller":"etcdserver/zap_raft.go:77","msg":"ed08be6981de8fb2 became follower at term 2"}
        {"level":"info","ts":"2024-01-07T18:21:46.078+0100","logger":"raft","caller":"etcdserver/zap_raft.go:77","msg":"ed08be6981de8fb2 [logterm: 1, index: 5, vote: 0] cast MsgVote for a5227d6d18885972 [logterm: 1, index: 5] at term 2"}
        {"level":"info","ts":"2024-01-07T18:21:46.090+0100","logger":"raft","caller":"etcdserver/zap_raft.go:77","msg":"raft.node: ed08be6981de8fb2 elected leader a5227d6d18885972 at term 2"}
        {"level":"info","ts":"2024-01-07T18:21:46.283+0100","caller":"rafthttp/peer_status.go:53","msg":"peer became active","peer-id":"a892a28439628834"}
        {"level":"info","ts":"2024-01-07T18:21:46.283+0100","caller":"rafthttp/stream.go:412","msg":"established TCP streaming connection with remote peer","stream-reader-type":"stream Message","local-member-id":"ed08be6981de8fb2","remote-peer-id":"a892a28439628834"}
        {"level":"info","ts":"2024-01-07T18:21:46.286+0100","caller":"rafthttp/stream.go:412","msg":"established TCP streaming connection with remote peer","stream-reader-type":"stream MsgApp v2","local-member-id":"ed08be6981de8fb2","remote-peer-id":"a892a28439628834"}
        {"level":"info","ts":"2024-01-07T18:21:46.340+0100","logger":"raft","caller":"etcdserver/zap_raft.go:77","msg":"ed08be6981de8fb2 [logterm: 2, index: 6, vote: a5227d6d18885972] ignored MsgPreVote from a892a28439628834 [logterm: 1, index: 5] at term 2: lease is not expired (remaining ticks: 2)"}
        {"level":"info","ts":"2024-01-07T18:21:47.208+0100","logger":"raft","caller":"etcdserver/zap_raft.go:77","msg":"ed08be6981de8fb2 [logterm: 2, index: 7, vote: a5227d6d18885972] ignored MsgPreVote from a892a28439628834 [logterm: 2, index: 6] at term 2: lease is not expired (remaining ticks: 2)"}
        {"level":"info","ts":"2024-01-07T18:21:47.670+0100","caller":"etcdserver/server.go:2027","msg":"published local member to cluster through raft","local-member-id":"ed08be6981de8fb2","local-member-attributes":"{Name:node2 ClientURLs:[http://127.0.1.1:2482]}","request-path":"/0/members/ed08be6981de8fb2/attributes","cluster-id":"60494380bb9bd798","publish-timeout":"6s"}
        {"level":"info","ts":"2024-01-07T18:21:47.672+0100","caller":"embed/serve.go:98","msg":"ready to serve client requests"}
        {"level":"info","ts":"2024-01-07T18:21:47.673+0100","caller":"etcdmain/main.go:47","msg":"notifying init daemon"}
        {"level":"info","ts":"2024-01-07T18:21:47.673+0100","caller":"etcdmain/main.go:53","msg":"successfully notified init daemon"}
        {"level":"info","ts":"2024-01-07T18:21:47.674+0100","caller":"embed/serve.go:140","msg":"serving client traffic insecurely; this is strongly discouraged!","address":"127.0.1.1:2482"}
        {"level":"info","ts":"2024-01-07T18:21:47.953+0100","caller":"rafthttp/stream.go:249","msg":"set message encoder","from":"ed08be6981de8fb2","to":"48ea5f4fca1499b","stream-type":"stream MsgApp v2"}
        {"level":"info","ts":"2024-01-07T18:21:47.955+0100","caller":"rafthttp/peer_status.go:53","msg":"peer became active","peer-id":"48ea5f4fca1499b"}
        {"level":"info","ts":"2024-01-07T18:21:47.958+0100","caller":"rafthttp/stream.go:274","msg":"established TCP streaming connection with remote peer","stream-writer-type":"stream MsgApp v2","local-member-id":"ed08be6981de8fb2","remote-peer-id":"48ea5f4fca1499b"}
        get file path socket:[1059761061]
        inject fail to process 541578 syscall 1
        {"level":"info","ts":"2024-01-07T18:21:48.073+0100","caller":"rafthttp/stream.go:249","msg":"set message encoder","from":"ed08be6981de8fb2","to":"a5227d6d18885972","stream-type":"stream Message"}
        {"level":"warn","ts":"2024-01-07T18:21:48.073+0100","caller":"rafthttp/stream.go:265","msg":"closed TCP streaming connection with remote peer","stream-writer-type":"stream Message","local-member-id":"ed08be6981de8fb2","remote-peer-id":"a5227d6d18885972"}
        {"level":"info","ts":"2024-01-07T18:21:48.074+0100","caller":"rafthttp/stream.go:274","msg":"established TCP streaming connection with remote peer","stream-writer-type":"stream Message","local-member-id":"ed08be6981de8fb2","remote-peer-id":"a5227d6d18885972"}
        {"level":"info","ts":"2024-01-07T18:21:48.698+0100","caller":"rafthttp/stream.go:412","msg":"established TCP streaming connection with remote peer","stream-reader-type":"stream MsgApp v2","local-member-id":"ed08be6981de8fb2","remote-peer-id":"48ea5f4fca1499b"}
        {"level":"info","ts":"2024-01-07T18:21:48.703+0100","caller":"rafthttp/stream.go:412","msg":"established TCP streaming connection with remote peer","stream-reader-type":"stream Message","local-member-id":"ed08be6981de8fb2","remote-peer-id":"48ea5f4fca1499b"}
        panic: runtime error: slice bounds out of range [2:1]

        goroutine 174 [running]:
        go.etcd.io/etcd/raft/v3.(*raftLog).maybeAppend(0xc00043a000, 0x9, 0x2, 0x9, 0xc0005893b0, 0x1, 0x1, 0x0, 0x0)
                /tmp/etcd-release-3.5.1/etcd/release/etcd/raft/log.go:98 +0x26f
        go.etcd.io/etcd/raft/v3.(*raft).handleAppendEntries(0xc0004189a0, 0x3, 0xed08be6981de8fb2, 0xa5227d6d18885972, 0x2, 0x2, 0x9, 0xc0005893b0, 0x1, 0x1, ...)
                /tmp/etcd-release-3.5.1/etcd/release/etcd/raft/raft.go:1475 +0x9f
        go.etcd.io/etcd/raft/v3.stepFollower(0xc0004189a0, 0x3, 0xed08be6981de8fb2, 0xa5227d6d18885972, 0x2, 0x2, 0x9, 0xc0005893b0, 0x1, 0x1, ...)
                /tmp/etcd-release-3.5.1/etcd/release/etcd/raft/raft.go:1430 +0x3f8
        go.etcd.io/etcd/raft/v3.(*raft).Step(0xc0004189a0, 0x3, 0xed08be6981de8fb2, 0xa5227d6d18885972, 0x2, 0x2, 0x9, 0xc0005893b0, 0x1, 0x1, ...)
                /tmp/etcd-release-3.5.1/etcd/release/etcd/raft/raft.go:975 +0xa55
        go.etcd.io/etcd/raft/v3.(*node).run(0xc000130240)
                /tmp/etcd-release-3.5.1/etcd/release/etcd/raft/node.go:356 +0x798
        created by go.etcd.io/etcd/raft/v3.StartNode
                /tmp/etcd-release-3.5.1/etcd/release/etcd/raft/node.go:230 +0x375
        couraged!","address":"127.0.1.1:2482"
    ```
2. etcd-3517

3. tocommit() is out of range

    ```
        {"level":"info","ts":"2024-01-07T19:15:23.577+0100","caller":"etcdmain/etcd.go:72","msg":"Running: ","args":["/home/zyh/etcd/bin/etcd","--name","node2","--data-dir","./data2","--heartbeat-interval","100","--election-timeout","500","--socket-reuse-port","--socket-reuse-address","--initial-cluster-token","etcd-cluster","--initial-cluster-state","new","--initial-advertise-peer-urls","http://127.0.1.1:2382","--listen-peer-urls","http://127.0.1.1:2382","--listen-client-urls","http://127.0.1.1:2482","--advertise-client-urls","http://127.0.1.1:2482","--initial-cluster","node0=http://127.0.1.1:2380,node1=http://127.0.1.1:2381,node2=http://127.0.1.1:2382,node3=http://127.0.1.1:2383,node4=http://127.0.1.1:2384"]}
        {"level":"info","ts":"2024-01-07T19:15:23.578+0100","caller":"etcdmain/etcd.go:99","msg":"failed to detect default host","error":"could not find default route"}
        {"level":"info","ts":"2024-01-07T19:15:23.578+0100","caller":"embed/etcd.go:125","msg":"configuring socket options","reuse-address":true,"reuse-port":true}
        {"level":"info","ts":"2024-01-07T19:15:23.578+0100","caller":"embed/etcd.go:131","msg":"configuring peer listeners","listen-peer-urls":["http://127.0.1.1:2382"]}
        {"level":"info","ts":"2024-01-07T19:15:23.579+0100","caller":"embed/etcd.go:139","msg":"configuring client listeners","listen-client-urls":["http://127.0.1.1:2482"]}
        {"level":"info","ts":"2024-01-07T19:15:23.579+0100","caller":"embed/etcd.go:307","msg":"starting an etcd server","etcd-version":"3.5.1","git-sha":"e8732fb5f","go-version":"go1.16.3","go-os":"linux","go-arch":"amd64","max-cpu-set":48,"max-cpu-available":48,"member-initialized":false,"name":"node2","data-dir":"./data2","wal-dir":"","wal-dir-dedicated":"","member-dir":"data2/member","force-new-cluster":false,"heartbeat-interval":"100ms","election-timeout":"500ms","initial-election-tick-advance":true,"snapshot-count":100000,"snapshot-catchup-entries":5000,"initial-advertise-peer-urls":["http://127.0.1.1:2382"],"listen-peer-urls":["http://127.0.1.1:2382"],"advertise-client-urls":["http://127.0.1.1:2482"],"listen-client-urls":["http://127.0.1.1:2482"],"listen-metrics-urls":[],"cors":["*"],"host-whitelist":["*"],"initial-cluster":"node0=http://127.0.1.1:2380,node1=http://127.0.1.1:2381,node2=http://127.0.1.1:2382,node3=http://127.0.1.1:2383,node4=http://127.0.1.1:2384","initial-cluster-state":"new","initial-cluster-token":"etcd-cluster","quota-size-bytes":2147483648,"pre-vote":true,"initial-corrupt-check":false,"corrupt-check-time-interval":"0s","auto-compaction-mode":"periodic","auto-compaction-retention":"0s","auto-compaction-interval":"0s","discovery-url":"","discovery-proxy":"","downgrade-check-interval":"5s"}
        {"level":"info","ts":"2024-01-07T19:15:23.670+0100","caller":"etcdserver/backend.go:81","msg":"opened backend db","path":"data2/member/snap/db","took":"88.911326ms"}
        {"level":"info","ts":"2024-01-07T19:15:23.730+0100","caller":"etcdserver/raft.go:448","msg":"starting local member","local-member-id":"ed08be6981de8fb2","cluster-id":"60494380bb9bd798"}
        {"level":"info","ts":"2024-01-07T19:15:23.731+0100","logger":"raft","caller":"etcdserver/zap_raft.go:77","msg":"ed08be6981de8fb2 switched to configuration voters=()"}
        {"level":"info","ts":"2024-01-07T19:15:23.731+0100","logger":"raft","caller":"etcdserver/zap_raft.go:77","msg":"ed08be6981de8fb2 became follower at term 0"}
        {"level":"info","ts":"2024-01-07T19:15:23.731+0100","logger":"raft","caller":"etcdserver/zap_raft.go:77","msg":"newRaft ed08be6981de8fb2 [peers: [], term: 0, commit: 0, applied: 0, lastindex: 0, lastterm: 0]"}
        {"level":"info","ts":"2024-01-07T19:15:23.731+0100","logger":"raft","caller":"etcdserver/zap_raft.go:77","msg":"ed08be6981de8fb2 became follower at term 1"}
        {"level":"info","ts":"2024-01-07T19:15:23.731+0100","logger":"raft","caller":"etcdserver/zap_raft.go:77","msg":"ed08be6981de8fb2 switched to configuration voters=(328382294473656731)"}
        {"level":"info","ts":"2024-01-07T19:15:23.731+0100","logger":"raft","caller":"etcdserver/zap_raft.go:77","msg":"ed08be6981de8fb2 switched to configuration voters=(328382294473656731 11899211072982767986)"}
        {"level":"info","ts":"2024-01-07T19:15:23.731+0100","logger":"raft","caller":"etcdserver/zap_raft.go:77","msg":"ed08be6981de8fb2 switched to configuration voters=(328382294473656731 11899211072982767986 12146949833753790516)"}
        {"level":"info","ts":"2024-01-07T19:15:23.731+0100","logger":"raft","caller":"etcdserver/zap_raft.go:77","msg":"ed08be6981de8fb2 switched to configuration voters=(328382294473656731 11899211072982767986 12146949833753790516 16839856138499966233)"}
        {"level":"info","ts":"2024-01-07T19:15:23.731+0100","logger":"raft","caller":"etcdserver/zap_raft.go:77","msg":"ed08be6981de8fb2 switched to configuration voters=(328382294473656731 11899211072982767986 12146949833753790516 16839856138499966233 17080110947162296242)"}
        {"level":"warn","ts":"2024-01-07T19:15:23.733+0100","caller":"auth/store.go:1220","msg":"simple token is not cryptographically signed"}
        {"level":"info","ts":"2024-01-07T19:15:23.736+0100","caller":"mvcc/kvstore.go:415","msg":"kvstore restored","current-rev":1}
        {"level":"info","ts":"2024-01-07T19:15:23.739+0100","caller":"etcdserver/quota.go:94","msg":"enabled backend quota with default value","quota-name":"v3-applier","quota-size-bytes":2147483648,"quota-size":"2.1 GB"}
        {"level":"info","ts":"2024-01-07T19:15:23.739+0100","caller":"rafthttp/peer.go:133","msg":"starting remote peer","remote-peer-id":"48ea5f4fca1499b"}
        {"level":"info","ts":"2024-01-07T19:15:23.740+0100","caller":"rafthttp/pipeline.go:72","msg":"started HTTP pipelining with remote peer","local-member-id":"ed08be6981de8fb2","remote-peer-id":"48ea5f4fca1499b"}
        {"level":"info","ts":"2024-01-07T19:15:23.740+0100","caller":"rafthttp/stream.go:169","msg":"started stream writer with remote peer","local-member-id":"ed08be6981de8fb2","remote-peer-id":"48ea5f4fca1499b"}
        {"level":"info","ts":"2024-01-07T19:15:23.741+0100","caller":"rafthttp/stream.go:169","msg":"started stream writer with remote peer","local-member-id":"ed08be6981de8fb2","remote-peer-id":"48ea5f4fca1499b"}
        {"level":"info","ts":"2024-01-07T19:15:23.741+0100","caller":"rafthttp/peer.go:137","msg":"started remote peer","remote-peer-id":"48ea5f4fca1499b"}
        {"level":"info","ts":"2024-01-07T19:15:23.741+0100","caller":"rafthttp/stream.go:395","msg":"started stream reader with remote peer","stream-reader-type":"stream MsgApp v2","local-member-id":"ed08be6981de8fb2","remote-peer-id":"48ea5f4fca1499b"}
        {"level":"info","ts":"2024-01-07T19:15:23.741+0100","caller":"rafthttp/stream.go:395","msg":"started stream reader with remote peer","stream-reader-type":"stream Message","local-member-id":"ed08be6981de8fb2","remote-peer-id":"48ea5f4fca1499b"}
        {"level":"info","ts":"2024-01-07T19:15:23.741+0100","caller":"rafthttp/transport.go:317","msg":"added remote peer","local-member-id":"ed08be6981de8fb2","remote-peer-id":"48ea5f4fca1499b","remote-peer-urls":["http://127.0.1.1:2384"]}
        {"level":"info","ts":"2024-01-07T19:15:23.742+0100","caller":"rafthttp/peer.go:133","msg":"starting remote peer","remote-peer-id":"a5227d6d18885972"}
        {"level":"info","ts":"2024-01-07T19:15:23.742+0100","caller":"rafthttp/pipeline.go:72","msg":"started HTTP pipelining with remote peer","local-member-id":"ed08be6981de8fb2","remote-peer-id":"a5227d6d18885972"}
        {"level":"info","ts":"2024-01-07T19:15:23.745+0100","caller":"rafthttp/stream.go:169","msg":"started stream writer with remote peer","local-member-id":"ed08be6981de8fb2","remote-peer-id":"a5227d6d18885972"}
        {"level":"info","ts":"2024-01-07T19:15:23.751+0100","caller":"rafthttp/stream.go:169","msg":"started stream writer with remote peer","local-member-id":"ed08be6981de8fb2","remote-peer-id":"a5227d6d18885972"}
        {"level":"info","ts":"2024-01-07T19:15:23.754+0100","caller":"rafthttp/peer.go:137","msg":"started remote peer","remote-peer-id":"a5227d6d18885972"}
        {"level":"info","ts":"2024-01-07T19:15:23.754+0100","caller":"rafthttp/stream.go:395","msg":"started stream reader with remote peer","stream-reader-type":"stream Message","local-member-id":"ed08be6981de8fb2","remote-peer-id":"a5227d6d18885972"}
        {"level":"info","ts":"2024-01-07T19:15:23.754+0100","caller":"rafthttp/transport.go:317","msg":"added remote peer","local-member-id":"ed08be6981de8fb2","remote-peer-id":"a5227d6d18885972","remote-peer-urls":["http://127.0.1.1:2380"]}
        {"level":"info","ts":"2024-01-07T19:15:23.754+0100","caller":"rafthttp/peer.go:133","msg":"starting remote peer","remote-peer-id":"a892a28439628834"}
        {"level":"info","ts":"2024-01-07T19:15:23.755+0100","caller":"rafthttp/pipeline.go:72","msg":"started HTTP pipelining with remote peer","local-member-id":"ed08be6981de8fb2","remote-peer-id":"a892a28439628834"}
        {"level":"info","ts":"2024-01-07T19:15:23.754+0100","caller":"rafthttp/stream.go:395","msg":"started stream reader with remote peer","stream-reader-type":"stream MsgApp v2","local-member-id":"ed08be6981de8fb2","remote-peer-id":"a5227d6d18885972"}
        {"level":"info","ts":"2024-01-07T19:15:23.755+0100","caller":"rafthttp/peer.go:137","msg":"started remote peer","remote-peer-id":"a892a28439628834"}
        {"level":"info","ts":"2024-01-07T19:15:23.756+0100","caller":"rafthttp/stream.go:169","msg":"started stream writer with remote peer","local-member-id":"ed08be6981de8fb2","remote-peer-id":"a892a28439628834"}
        {"level":"info","ts":"2024-01-07T19:15:23.756+0100","caller":"rafthttp/stream.go:169","msg":"started stream writer with remote peer","local-member-id":"ed08be6981de8fb2","remote-peer-id":"a892a28439628834"}
        {"level":"info","ts":"2024-01-07T19:15:23.756+0100","caller":"rafthttp/stream.go:395","msg":"started stream reader with remote peer","stream-reader-type":"stream MsgApp v2","local-member-id":"ed08be6981de8fb2","remote-peer-id":"a892a28439628834"}
        {"level":"info","ts":"2024-01-07T19:15:23.757+0100","caller":"rafthttp/transport.go:317","msg":"added remote peer","local-member-id":"ed08be6981de8fb2","remote-peer-id":"a892a28439628834","remote-peer-urls":["http://127.0.1.1:2381"]}
        {"level":"info","ts":"2024-01-07T19:15:23.757+0100","caller":"rafthttp/peer.go:133","msg":"starting remote peer","remote-peer-id":"e9b32fefc3f8c519"}
        {"level":"info","ts":"2024-01-07T19:15:23.757+0100","caller":"rafthttp/stream.go:395","msg":"started stream reader with remote peer","stream-reader-type":"stream Message","local-member-id":"ed08be6981de8fb2","remote-peer-id":"a892a28439628834"}
        {"level":"info","ts":"2024-01-07T19:15:23.758+0100","caller":"rafthttp/pipeline.go:72","msg":"started HTTP pipelining with remote peer","local-member-id":"ed08be6981de8fb2","remote-peer-id":"e9b32fefc3f8c519"}
        {"level":"info","ts":"2024-01-07T19:15:23.759+0100","caller":"rafthttp/stream.go:169","msg":"started stream writer with remote peer","local-member-id":"ed08be6981de8fb2","remote-peer-id":"e9b32fefc3f8c519"}
        {"level":"info","ts":"2024-01-07T19:15:23.762+0100","caller":"rafthttp/stream.go:169","msg":"started stream writer with remote peer","local-member-id":"ed08be6981de8fb2","remote-peer-id":"e9b32fefc3f8c519"}
        {"level":"info","ts":"2024-01-07T19:15:23.763+0100","caller":"rafthttp/peer.go:137","msg":"started remote peer","remote-peer-id":"e9b32fefc3f8c519"}
        {"level":"info","ts":"2024-01-07T19:15:23.763+0100","caller":"rafthttp/transport.go:317","msg":"added remote peer","local-member-id":"ed08be6981de8fb2","remote-peer-id":"e9b32fefc3f8c519","remote-peer-urls":["http://127.0.1.1:2383"]}
        {"level":"info","ts":"2024-01-07T19:15:23.764+0100","caller":"rafthttp/stream.go:395","msg":"started stream reader with remote peer","stream-reader-type":"stream MsgApp v2","local-member-id":"ed08be6981de8fb2","remote-peer-id":"e9b32fefc3f8c519"}
        {"level":"info","ts":"2024-01-07T19:15:23.765+0100","caller":"rafthttp/stream.go:395","msg":"started stream reader with remote peer","stream-reader-type":"stream Message","local-member-id":"ed08be6981de8fb2","remote-peer-id":"e9b32fefc3f8c519"}
        {"level":"info","ts":"2024-01-07T19:15:23.764+0100","caller":"etcdserver/server.go:843","msg":"starting etcd server","local-member-id":"ed08be6981de8fb2","local-server-version":"3.5.1","cluster-version":"to_be_decided"}
        {"level":"info","ts":"2024-01-07T19:15:23.766+0100","caller":"etcdserver/server.go:744","msg":"starting initial election tick advance","election-ticks":5}
        {"level":"info","ts":"2024-01-07T19:15:23.767+0100","logger":"raft","caller":"etcdserver/zap_raft.go:77","msg":"ed08be6981de8fb2 switched to configuration voters=(328382294473656731 11899211072982767986 12146949833753790516 16839856138499966233 17080110947162296242)"}
        get file path socket:[1063175121]
        inject delay to process 681001 syscall 0
        {"level":"info","ts":"2024-01-07T19:15:23.769+0100","caller":"membership/cluster.go:421","msg":"added member","cluster-id":"60494380bb9bd798","local-member-id":"ed08be6981de8fb2","added-peer-id":"48ea5f4fca1499b","added-peer-peer-urls":["http://127.0.1.1:2384"]}
        {"level":"info","ts":"2024-01-07T19:15:23.772+0100","logger":"raft","caller":"etcdserver/zap_raft.go:77","msg":"ed08be6981de8fb2 switched to configuration voters=(328382294473656731 11899211072982767986 12146949833753790516 16839856138499966233 17080110947162296242)"}
        {"level":"info","ts":"2024-01-07T19:15:23.773+0100","caller":"rafthttp/peer_status.go:53","msg":"peer became active","peer-id":"a5227d6d18885972"}
        {"level":"info","ts":"2024-01-07T19:15:23.773+0100","caller":"rafthttp/stream.go:412","msg":"established TCP streaming connection with remote peer","stream-reader-type":"stream Message","local-member-id":"ed08be6981de8fb2","remote-peer-id":"a5227d6d18885972"}
        {"level":"info","ts":"2024-01-07T19:15:23.774+0100","caller":"membership/cluster.go:421","msg":"added member","cluster-id":"60494380bb9bd798","local-member-id":"ed08be6981de8fb2","added-peer-id":"a5227d6d18885972","added-peer-peer-urls":["http://127.0.1.1:2380"]}
        {"level":"info","ts":"2024-01-07T19:15:23.776+0100","logger":"raft","caller":"etcdserver/zap_raft.go:77","msg":"ed08be6981de8fb2 switched to configuration voters=(328382294473656731 11899211072982767986 12146949833753790516 16839856138499966233 17080110947162296242)"}
        {"level":"info","ts":"2024-01-07T19:15:23.777+0100","caller":"membership/cluster.go:421","msg":"added member","cluster-id":"60494380bb9bd798","local-member-id":"ed08be6981de8fb2","added-peer-id":"a892a28439628834","added-peer-peer-urls":["http://127.0.1.1:2381"]}
        {"level":"info","ts":"2024-01-07T19:15:23.777+0100","caller":"embed/etcd.go:276","msg":"now serving peer/client/metrics","local-member-id":"ed08be6981de8fb2","initial-advertise-peer-urls":["http://127.0.1.1:2382"],"listen-peer-urls":["http://127.0.1.1:2382"],"advertise-client-urls":["http://127.0.1.1:2482"],"listen-client-urls":["http://127.0.1.1:2482"],"listen-metrics-urls":[]}
        {"level":"info","ts":"2024-01-07T19:15:23.777+0100","caller":"embed/etcd.go:580","msg":"serving peer traffic","address":"127.0.1.1:2382"}
        {"level":"info","ts":"2024-01-07T19:15:23.777+0100","caller":"embed/etcd.go:552","msg":"cmux::serve","address":"127.0.1.1:2382"}
        {"level":"info","ts":"2024-01-07T19:15:23.777+0100","logger":"raft","caller":"etcdserver/zap_raft.go:77","msg":"ed08be6981de8fb2 switched to configuration voters=(328382294473656731 11899211072982767986 12146949833753790516 16839856138499966233 17080110947162296242)"}
        {"level":"info","ts":"2024-01-07T19:15:23.777+0100","caller":"membership/cluster.go:421","msg":"added member","cluster-id":"60494380bb9bd798","local-member-id":"ed08be6981de8fb2","added-peer-id":"e9b32fefc3f8c519","added-peer-peer-urls":["http://127.0.1.1:2383"]}
        {"level":"info","ts":"2024-01-07T19:15:23.777+0100","logger":"raft","caller":"etcdserver/zap_raft.go:77","msg":"ed08be6981de8fb2 switched to configuration voters=(328382294473656731 11899211072982767986 12146949833753790516 16839856138499966233 17080110947162296242)"}
        {"level":"info","ts":"2024-01-07T19:15:23.778+0100","caller":"membership/cluster.go:421","msg":"added member","cluster-id":"60494380bb9bd798","local-member-id":"ed08be6981de8fb2","added-peer-id":"ed08be6981de8fb2","added-peer-peer-urls":["http://127.0.1.1:2382"]}
        {"level":"info","ts":"2024-01-07T19:15:23.784+0100","caller":"rafthttp/stream.go:249","msg":"set message encoder","from":"ed08be6981de8fb2","to":"a5227d6d18885972","stream-type":"stream MsgApp v2"}
        {"level":"info","ts":"2024-01-07T19:15:23.785+0100","caller":"rafthttp/stream.go:274","msg":"established TCP streaming connection with remote peer","stream-writer-type":"stream MsgApp v2","local-member-id":"ed08be6981de8fb2","remote-peer-id":"a5227d6d18885972"}
        {"level":"info","ts":"2024-01-07T19:15:23.790+0100","caller":"rafthttp/stream.go:412","msg":"established TCP streaming connection with remote peer","stream-reader-type":"stream MsgApp v2","local-member-id":"ed08be6981de8fb2","remote-peer-id":"a5227d6d18885972"}
        {"level":"info","ts":"2024-01-07T19:15:23.790+0100","caller":"rafthttp/stream.go:249","msg":"set message encoder","from":"ed08be6981de8fb2","to":"a5227d6d18885972","stream-type":"stream Message"}
        {"level":"info","ts":"2024-01-07T19:15:23.794+0100","caller":"rafthttp/stream.go:274","msg":"established TCP streaming connection with remote peer","stream-writer-type":"stream Message","local-member-id":"ed08be6981de8fb2","remote-peer-id":"a5227d6d18885972"}
        get file path socket:[1063187965]
        inject delay to process 680928 syscall 0
        {"level":"info","ts":"2024-01-07T19:15:23.796+0100","caller":"rafthttp/peer_status.go:53","msg":"peer became active","peer-id":"a892a28439628834"}
        {"level":"info","ts":"2024-01-07T19:15:23.796+0100","caller":"rafthttp/stream.go:412","msg":"established TCP streaming connection with remote peer","stream-reader-type":"stream MsgApp v2","local-member-id":"ed08be6981de8fb2","remote-peer-id":"a892a28439628834"}
        {"level":"info","ts":"2024-01-07T19:15:23.798+0100","caller":"rafthttp/stream.go:249","msg":"set message encoder","from":"ed08be6981de8fb2","to":"a892a28439628834","stream-type":"stream Message"}
        {"level":"info","ts":"2024-01-07T19:15:23.800+0100","caller":"rafthttp/stream.go:274","msg":"established TCP streaming connection with remote peer","stream-writer-type":"stream Message","local-member-id":"ed08be6981de8fb2","remote-peer-id":"a892a28439628834"}
        {"level":"info","ts":"2024-01-07T19:15:23.802+0100","caller":"rafthttp/stream.go:412","msg":"established TCP streaming connection with remote peer","stream-reader-type":"stream Message","local-member-id":"ed08be6981de8fb2","remote-peer-id":"a892a28439628834"}
        get file path socket:[1063185009]
        inject fail to process 680974 syscall 0
        get file path socket:[1063202415]
        inject fail to process 680957 syscall 1
        get file path socket:[1063185009]
        inject fail to process 680974 syscall 0
        get file path socket:[1063202415]
        inject fail to process 680928 syscall 0
        {"level":"info","ts":"2024-01-07T19:15:23.809+0100","caller":"rafthttp/stream.go:249","msg":"set message encoder","from":"ed08be6981de8fb2","to":"a892a28439628834","stream-type":"stream MsgApp v2"}
        {"level":"info","ts":"2024-01-07T19:15:23.810+0100","caller":"rafthttp/stream.go:274","msg":"established TCP streaming connection with remote peer","stream-writer-type":"stream MsgApp v2","local-member-id":"ed08be6981de8fb2","remote-peer-id":"a892a28439628834"}
        get file path socket:[1063185010]
        inject fail to process 680974 syscall 0
        get file path socket:[1063185010]
        inject fail to process 680974 syscall 0
        get file path socket:[1063185011]
        inject fail to process 680957 syscall 0
        get file path socket:[1063174045]
        inject fail to process 680993 syscall 0
        get file path socket:[1063185011]
        inject fail to process 680957 syscall 0
        get file path socket:[1063185012]
        inject fail to process 680957 syscall 0
        get file path socket:[1063185012]
        inject delay to process 680957 syscall 0
        {"level":"info","ts":"2024-01-07T19:15:23.818+0100","caller":"etcdserver/server.go:767","msg":"initialized peer connections; fast-forwarding election ticks","local-member-id":"ed08be6981de8fb2","forward-ticks":3,"forward-duration":"300ms","election-ticks":5,"election-timeout":"500ms","active-remote-members":2}
        {"level":"info","ts":"2024-01-07T19:15:23.820+0100","caller":"rafthttp/peer_status.go:53","msg":"peer became active","peer-id":"48ea5f4fca1499b"}
        get file path socket:[1063182130]
        inject fail to process 680957 syscall 0
        {"level":"info","ts":"2024-01-07T19:15:23.823+0100","caller":"rafthttp/stream.go:412","msg":"established TCP streaming connection with remote peer","stream-reader-type":"stream Message","local-member-id":"ed08be6981de8fb2","remote-peer-id":"48ea5f4fca1499b"}
        get file path socket:[1063207186]
        inject fail to process 680993 syscall 0
        get file path socket:[1063207186]
        inject fail to process 680993 syscall 0
        get file path socket:[1063207186]
        inject fail to process 680960 syscall 1
        get file path socket:[1063190904]
        inject fail to process 680928 syscall 0
        {"level":"info","ts":"2024-01-07T19:15:23.823+0100","caller":"rafthttp/stream.go:412","msg":"established TCP streaming connection with remote peer","stream-reader-type":"stream MsgApp v2","local-member-id":"ed08be6981de8fb2","remote-peer-id":"48ea5f4fca1499b"}
        get file path socket:[1063173043]
        inject fail to process 680993 syscall 0
        {"level":"warn","ts":"2024-01-07T19:15:23.828+0100","caller":"rafthttp/stream.go:421","msg":"lost TCP streaming connection with remote peer","stream-reader-type":"stream Message","local-member-id":"ed08be6981de8fb2","remote-peer-id":"48ea5f4fca1499b","error":"read tcp 127.0.0.1:41906->127.0.1.1:2384: read: invalid argument"}
        get file path socket:[1063192057]
        inject fail to process 680927 syscall 0
        {"level":"warn","ts":"2024-01-07T19:15:23.830+0100","caller":"rafthttp/peer_status.go:66","msg":"peer became inactive (message send to peer failed)","peer-id":"48ea5f4fca1499b","error":"failed to read 48ea5f4fca1499b on stream Message (read tcp 127.0.0.1:41906->127.0.1.1:2384: read: invalid argument)"}
        {"level":"warn","ts":"2024-01-07T19:15:23.828+0100","caller":"rafthttp/stream.go:421","msg":"lost TCP streaming connection with remote peer","stream-reader-type":"stream MsgApp v2","local-member-id":"ed08be6981de8fb2","remote-peer-id":"48ea5f4fca1499b","error":"read tcp 127.0.0.1:41896->127.0.1.1:2384: read: invalid argument"}
        get file path socket:[1063192057]
        inject fail to process 680927 syscall 0
        get file path socket:[1063192058]
        inject fail to process 680927 syscall 0
        get file path socket:[1063192058]
        inject fail to process 680927 syscall 1
        get file path socket:[1063193069]
        inject fail to process 680957 syscall 0
        get file path socket:[1063214291]
        inject fail to process 680915 syscall 1
        get file path socket:[1063216257]
        inject fail to process 680960 syscall 0
        get file path socket:[1063216257]
        inject fail to process 680927 syscall 1
        get file path socket:[1063214291]
        inject fail to process 680928 syscall 0
        get file path socket:[1063193069]
        inject fail to process 680957 syscall 0
        get file path socket:[1063188905]
        inject fail to process 680915 syscall 0
        {"level":"info","ts":"2024-01-07T19:15:23.896+0100","caller":"rafthttp/stream.go:249","msg":"set message encoder","from":"ed08be6981de8fb2","to":"48ea5f4fca1499b","stream-type":"stream MsgApp v2"}
        {"level":"info","ts":"2024-01-07T19:15:23.898+0100","caller":"rafthttp/peer_status.go:53","msg":"peer became active","peer-id":"48ea5f4fca1499b"}
        {"level":"info","ts":"2024-01-07T19:15:23.898+0100","caller":"rafthttp/stream.go:274","msg":"established TCP streaming connection with remote peer","stream-writer-type":"stream MsgApp v2","local-member-id":"ed08be6981de8fb2","remote-peer-id":"48ea5f4fca1499b"}
        {"level":"info","ts":"2024-01-07T19:15:23.898+0100","caller":"rafthttp/peer_status.go:53","msg":"peer became active","peer-id":"e9b32fefc3f8c519"}
        {"level":"info","ts":"2024-01-07T19:15:23.898+0100","caller":"rafthttp/stream.go:412","msg":"established TCP streaming connection with remote peer","stream-reader-type":"stream Message","local-member-id":"ed08be6981de8fb2","remote-peer-id":"e9b32fefc3f8c519"}
        {"level":"info","ts":"2024-01-07T19:15:23.908+0100","caller":"rafthttp/stream.go:249","msg":"set message encoder","from":"ed08be6981de8fb2","to":"48ea5f4fca1499b","stream-type":"stream Message"}
        {"level":"info","ts":"2024-01-07T19:15:23.909+0100","caller":"rafthttp/stream.go:274","msg":"established TCP streaming connection with remote peer","stream-writer-type":"stream Message","local-member-id":"ed08be6981de8fb2","remote-peer-id":"48ea5f4fca1499b"}
        {"level":"info","ts":"2024-01-07T19:15:23.908+0100","logger":"raft","caller":"etcdserver/zap_raft.go:77","msg":"ed08be6981de8fb2 [logterm: 1, index: 5, vote: 0] cast MsgPreVote for a5227d6d18885972 [logterm: 1, index: 5] at term 1"}
        get file path socket:[1063213249]
        inject delay to process 680915 syscall 0
        {"level":"info","ts":"2024-01-07T19:15:23.926+0100","logger":"raft","caller":"etcdserver/zap_raft.go:77","msg":"ed08be6981de8fb2 [term: 1] received a MsgVote message with higher term from a5227d6d18885972 [term: 2]"}
        {"level":"info","ts":"2024-01-07T19:15:23.927+0100","logger":"raft","caller":"etcdserver/zap_raft.go:77","msg":"ed08be6981de8fb2 became follower at term 2"}
        {"level":"info","ts":"2024-01-07T19:15:23.927+0100","logger":"raft","caller":"etcdserver/zap_raft.go:77","msg":"ed08be6981de8fb2 [logterm: 1, index: 5, vote: 0] cast MsgVote for a5227d6d18885972 [logterm: 1, index: 5] at term 2"}
        {"level":"info","ts":"2024-01-07T19:15:23.926+0100","caller":"rafthttp/stream.go:249","msg":"set message encoder","from":"ed08be6981de8fb2","to":"e9b32fefc3f8c519","stream-type":"stream MsgApp v2"}
        {"level":"info","ts":"2024-01-07T19:15:23.927+0100","caller":"rafthttp/stream.go:274","msg":"established TCP streaming connection with remote peer","stream-writer-type":"stream MsgApp v2","local-member-id":"ed08be6981de8fb2","remote-peer-id":"e9b32fefc3f8c519"}
        {"level":"info","ts":"2024-01-07T19:15:23.932+0100","caller":"rafthttp/stream.go:412","msg":"established TCP streaming connection with remote peer","stream-reader-type":"stream MsgApp v2","local-member-id":"ed08be6981de8fb2","remote-peer-id":"e9b32fefc3f8c519"}
        get file path socket:[1063196035]
        inject delay to process 680928 syscall 0
        {"level":"info","ts":"2024-01-07T19:15:23.943+0100","caller":"rafthttp/stream.go:249","msg":"set message encoder","from":"ed08be6981de8fb2","to":"e9b32fefc3f8c519","stream-type":"stream Message"}
        {"level":"info","ts":"2024-01-07T19:15:23.945+0100","caller":"rafthttp/stream.go:274","msg":"established TCP streaming connection with remote peer","stream-writer-type":"stream Message","local-member-id":"ed08be6981de8fb2","remote-peer-id":"e9b32fefc3f8c519"}
        {"level":"info","ts":"2024-01-07T19:15:23.947+0100","logger":"raft","caller":"etcdserver/zap_raft.go:77","msg":"raft.node: ed08be6981de8fb2 elected leader a5227d6d18885972 at term 2"}
        {"level":"info","ts":"2024-01-07T19:15:23.949+0100","caller":"rafthttp/stream.go:412","msg":"established TCP streaming connection with remote peer","stream-reader-type":"stream MsgApp v2","local-member-id":"ed08be6981de8fb2","remote-peer-id":"48ea5f4fca1499b"}
        {"level":"info","ts":"2024-01-07T19:15:23.953+0100","caller":"rafthttp/stream.go:412","msg":"established TCP streaming connection with remote peer","stream-reader-type":"stream Message","local-member-id":"ed08be6981de8fb2","remote-peer-id":"48ea5f4fca1499b"}
        {"level":"info","ts":"2024-01-07T19:15:24.003+0100","caller":"etcdserver/server.go:2027","msg":"published local member to cluster through raft","local-member-id":"ed08be6981de8fb2","local-member-attributes":"{Name:node2 ClientURLs:[http://127.0.1.1:2482]}","request-path":"/0/members/ed08be6981de8fb2/attributes","cluster-id":"60494380bb9bd798","publish-timeout":"6s"}
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
    ```