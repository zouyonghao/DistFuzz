# We use the following ${SYSTEM}-test repos to simplify the build process.
# braft https://github.com/zouyonghao/braft-test
# AerospikeDB https://github.com/zouyonghao/aerospikedb-test.git
# Dqlite https://github.com/zouyonghao/dqlite-test.git
# RethinkDB https://github.com/zouyonghao/rethinkdb-test.git

# We use a newer version of etcd because the old version in the paper is buggy and is only used for comparison purpose.
# etcd https://github.com/etcd-io/etcd/releases/download/v3.5.1/etcd-v3.5.1-linux-amd64.tar.gz

# We use a newer version of ZooKeeper because the old version in the paper is buggy and is only used for comparison purpose.
# ZooKeeper https://archive.apache.org/dist/zookeeper/zookeeper-3.7.0/apache-zookeeper-3.7.0.tar.gz

# Base image
FROM ubuntu:20.04

# Install dependencies
RUN apt-get update && sudo apt-get install \
  libboost-all-dev \
  rapidjson-dev \
  clang-9 clang-11 llvm-11 \
  libssl-dev \
  libleveldb-dev \
  libgflags-dev \
  libprotoc-dev \
  libleveldb-dev \
  autoconf \
  libbsd-dev \
  libnet-dev \
  psmisc \
  libprotobuf-dev libprotobuf-c-dev protobuf-c-compiler protobuf-compiler python3-protobuf \
  python3-future libnl-3-dev libcap-dev \
  lsb-release wget software-properties-common \
  librdkafka-dev \
  redis \
  zookeeper \
  gcc-multilib \
  gawk gcc g++ libc++-dev liblzma-dev lzma-dev leiningen valgrind libelf1 libelf-dev zlib1g-dev bear \
  capnproto libcapnp-dev g++-multilib cmake tmux curl net-tools uml-utilities openjdk-11-jdk

# Install lein
RUN wget https://raw.githubusercontent.com/technomancy/leiningen/stable/bin/lein &&
  mv lein /usr/bin/lein &&
  chmod +x /usr/bin/lein

# Install aerospike-client-c
RUN wget https://download.aerospike.com/artifacts/aerospike-client-c/5.2.0/aerospike-client-c-5.2.0.ubuntu18.04.x86_64.tgz &&
  tar xvf aerospike-client-c-5.2.0.ubuntu18.04.x86_64.tgz &&
  cd aerospike-client-c-5.2.0.ubuntu18.04.x86_64 &&
  sudo dpkg -i aerospike-client-c-devel-5.2.0.ubuntu18.04.x86_64.deb

# Install criu
RUN https://github.com/checkpoint-restore/criu/archive/refs/tags/v4.0.tar.gz &&
  tar xvf v4.0.tar.gz &&
  cd criu-4.0 &&
  make &&
  make install