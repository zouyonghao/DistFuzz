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

`redisraft` 需要 `clang-9`，因此必须修改本项目的`CMakeLists.txt`为`llvm-9`和`clang-9`

`rethinkdb` 编译时需要联网下载部分组件

`ClickHouse` 暂未在 71.37 的 `docker` 中运行，仅在 71.79 的虚拟机上运行

`ClickHouse` 代码位于`ftp`上，使用下列方法获取

```
smbclient '\\166.111.139.139\workspace' -N -c 'prompt OFF; cd zyh; mget ClickHouse_21.9.2.17.tar.gz'
```

`ClickHouse` 需要新版本`cmake`，使用如下方式安装

```
wget https://github.com/Kitware/CMake/releases/download/v3.21.3/cmake-3.21.3-linux-x86_64.sh
chmod +x cmake-3.21.3-linux-x86_64.sh
./cmake-3.21.3-linux-x86_64.sh
修改 .bashrc，加入cmake执行路径
```

**新增项目**

添加新测试项目时需要增加、修改的文件：

1. run_fuzz_server_normal.sh
   一般是用于测试应用是否能正常启动

2. run_fuzz_server.sh
   启动开始测试需要的所有服务 （TODO:改为在main里控制）

3. start_server[0-2].sh
   启动单个server

4. operators
   一般需要实现多个，包括启动/停止等

5. backup_test_case.sh
   用于备份和检查相关日志、数据