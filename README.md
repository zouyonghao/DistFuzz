分布式模糊测试

测试时首先构建 distributed-system-test 项目，然后测试某项目时再分别构建其对应工程，注意clone工程是必须要特定目录名称

一般对应项目中包含 `build.sh`，直接使用即可

| 项目          | 目录名           | 对应源码或下载地址                                                                      | version         | LOC  |
| ------------- | ---------------- | --------------------------------------------------------------------------------------- | --------------- | ---- |
| braft         | braft            | http://oslab:7800/zyh/braft, http://oslab:7800/zyh/brpc                                 | commit 0c5a5942 | 89K  |
| c-raft        | **raft**         | http://oslab:7800/zyh/craft                                                             | commit 37af7cdb | 75K  |
| nuraft        | **NuRaft**       | http://oslab:7800/zyh/nuraft                                                            | commit 5a7a40e5 | 149K |
| redisraft     | redisraft        | http://oslab:7800/zyh/redisraft, http://oslab:7800/zyh/redis                            | commit e18c3860 | 148K |
| floyd(raftis) | floyd            | https://github.com/Qihoo360/floyd                                                       |
| rethinkdb     | rethinkdb        | http://oslab:7800/zyh/rethinkdb                                                         | v2.4.1          | 271K |
| ClickHouse    | ClickHouse       | \\\\oslab\workspace\zyh\ClickHouse_21.9.2.17                                            | v21.9.2.17      | 557K |
| aerospike     | aerospike-server | http://oslab:7800/zyh/aerospike-server.git                                              | v5.6.0.4        | 376K |
| Zookeeper     | zookeeper        | \\\\oslab\workspace\zyh\apache-zookeeper-3.7.0-bin.tar.gz                               | v3.5.1          | 231K |
| etcd          | etcd             | https://github.com/etcd-io/etcd/releases/download/v3.5.1/etcd-v3.5.1-linux-amd64.tar.gz | v2.2.0          | 249K |

**注意**

* `floyd` 需要使用 `ubuntu 14.04`，`docker` 镜像为 `ubuntu_14.04_dst_floyd`

* `redisraft` 需要 `clang-9`，因此必须修改本项目的`CMakeLists.txt`为`llvm-9`和`clang-9`

* `rethinkdb` 编译时需要联网下载部分组件

* `ClickHouse` 暂未在 71.37 的 `docker` 中运行，仅在 71.79 的虚拟机上运行

* `ClickHouse` 代码位于`ftp`上，使用下列方法获取

   ```
   smbclient '\\oslab\workspace' -N -c 'prompt OFF; cd zyh; mget ClickHouse_21.9.2.17.tar.gz'
   ```

* `ClickHouse` 需要新版本`cmake`，使用如下方式安装

   ```
   wget https://github.com/Kitware/CMake/releases/download/v3.21.3/cmake-3.21.3-linux-x86_64.sh
   chmod +x cmake-3.21.3-linux-x86_64.sh
   ./cmake-3.21.3-linux-x86_64.sh
   修改 .bashrc，加入cmake执行路径
   ```

* `Zookeeper` 需要将`jar`包重命名为`zookeeper.jar`

* TODO: 如果使用checkpoint，strace应改为attach模式，类似启动NuRaft的方式。(see nuraft_test/src/node_manager.cpp)

**新增项目**

添加新测试项目时需要增加、修改的文件：

1. 实现`node_manager`
   
   主要是实现 `ServerConfigurationGenerator`

2. operators
   
   一般需要实现多个，主要是 `ClientConfigurationGenerator` 和自定义 operators

3. backup_test_case.sh
   
   用于备份和检查相关日志、数据