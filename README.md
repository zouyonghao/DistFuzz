## DistFuzz

### Description

DistFuzz is a research fuzzing tool to effectively find bugs in distributed systems. It supports different kinds of distributed system inputs including regular input, faults and timing interval, and uses network message sequences with symmetry-based pruning as fuzzing feedback. It can minimize the initialization time and reproduce bugs with checkpointing and offline record/replay.

### Current supported distributed systems

* Braft
* NuRaft
* Dqlite
* Redis
* RethinkDB
* AerospikeDB
* ClickHouse
* etcd
* ZooKeeper
* HDFS

### Installation

We recommend you to run the tool using the Docker image we provide.

```
docker run -it zouyonghao/distfuzz:artifact bash
```

If you want to build the Docker container or install the tool and dependencies yourself, please refer to [INSTALL.md](INSTALL.md).

### Usage

#### Fuzzing
For each system, there is a folder called `${SYSTEM}_test`, e.g., `braft_test`. You need to enter the folder `${SYSTEM}_test/bin` and use the script `fuzz.sh` to run the fuzzing test.

```bash
# For the systems evaluated in the paper:

# braft
cd DistFuzz/braft_test/bin && ./fuzz.sh

# AerospikeDB
cd DistFuzz/aerospike_test/bin && ./fuzz.sh

# Dqlite
cd DistFuzz/dqlite_test/bin && ./fuzz.sh

# RethinkDB
cd DistFuzz/rethinkdb_test/bin && ./fuzz.sh

# etcd
cd DistFuzz/etcd_test/bin && ./fuzz.sh

# ZooKeeper
cd DistFuzz/zookeeper_test/bin && ./fuzz.sh
```

#### Analyzing results

After fuzzing for a while, you can check error cases under `DistFuzz/${SYSTEM}_test/bin/test_cases` and fuzzing status in the file `DistFuzz/${SYSTEM}_test/bin/output/fuzzer1/plot-curve` (`bc:` for coverage).

#### Reproducing

To reproduce a bug, you need to copy the `init_random.txt` file in your target bug folder, e.g., `test_cases/100`, to the current folder. Then you need to modify and run the script `reproduce.sh`.

We provide example bug reproduction Docker images we used during our communication with developers. Please note that `rr` has requirements for CPU and our recent tested CPUs are `Intel i9-10980XE` and `Intel(R) Xeon(R) Gold 6248R`.

```bash
# For etcd-1 https://github.com/etcd-io/etcd/issues/13493
docker run -it --rm --cap-add=SYS_PTRACE --security-opt seccomp=unconfined zouyonghao/etcd-13493 bash
# The rr version installed is a modified version of DistFuzz.
rr replay -a /root/test_cases/3169/rr_rec_1_0/

# For etcd-2 https://github.com/etcd-io/raft/issues/18
docker run -it --rm --cap-add=SYS_PTRACE --security-opt seccomp=unconfined zouyonghao/etcd-10166 bash
rr replay -a /root/10166/rr_rec_2_0/

# For ZooKeeper-[1-8]
docker run -it --rm --cap-add=SYS_PTRACE --security-opt seccomp=unconfined zouyonghao/distfuzz:zookeeper-rr bash
rr replay -a /home/zyh/zookeeper-[1-8]
```

### Code Structure

* `core` contains the core libs of DistFuzz including shared memory, common events (like killing node), node manager and the main test logic.
* `fuzz` contains the fuzzer that managers seeds and shared memory.
* `strace` is the default fault injector and network sequence collector.
* `rr` is the modified version of rr-debugger. It is used for record/replay but now it can also directly be used as the fault inject and network sequence collector. So, it actually can replace `strace` in the new version. But it's slower than `strace`.
* `bpf` can also replace `strace` and has better performance as it uses BPF as the fault injector. But its fault injection feature is limited. For example, it can only delay network messages or file operations within ~50ms. This is due to the limitation of BPF.
* `${SYSTEM}_test/src` and `${SYSTEM}_test/include` contain the custom events for the specific system.
* `${SYSTEM}_test/bin` contains the scripts for the specific system.
* `general_test` works as a template for user to understand how to add a new target system to DistFuzz.

### Found bugs

Please check the [FOUND_BUGS.md](FOUND_BUGS.md)