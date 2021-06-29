分布式模糊测试

测试时首先构建 distributed-system-test 项目，然后测试某项目时再分别构建其对应工程，注意clone工程是必须要特定目录名称

一般对应项目中包含 `build.sh`，直接使用即可

| 项目          | 目录名     | 对应git                                                                          |
| ------------- | ---------- | -------------------------------------------------------------------------------- |
| braft         | braft      | http://166.111.139.139:7800/zyh/braft, http://166.111.139.139:7800/zyh/brpc      |
| c-raft        | **raft**   | http://166.111.139.139:7800/zyh/craft                                            |
| nuraft        | **NuRaft** | http://166.111.139.139:7800/zyh/nuraft                                           |
| redisraft     | redisraft  | http://166.111.139.139:7800/zyh/redisraft, http://166.111.139.139:7800/zyh/redis |
| floyd(raftis) | floyd      | https://github.com/Qihoo360/floyd                                                |
| rethinkdb     | rethinkdb  | http://166.111.139.139:7800/zyh/rethinkdb                                        |
| ClickHouse    | ClickHouse | https://github.com/ClickHouse/ClickHouse.git                                     |

**注意**

`floyd` 需要使用 `ubuntu 14.04`，`docker` 镜像为 `ubuntu_14.04_dst_floyd`

`rethinkdb` 编译时需要联网下载部分组件

`ClickHouse` 暂未在 71.37 的 `docker` 中运行，仅在 71.79 的虚拟机上运行