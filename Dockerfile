# Base image
FROM ubuntu:20.04

# Install dependencies
RUN apt update && \
  DEBIAN_FRONTEND=noninteractive apt install \
  build-essential \
  libboost-all-dev \
  rapidjson-dev \
  clang-9 \
  clang-11 \
  llvm-11 \
  libssl-dev \
  libleveldb-dev \
  libgflags-dev \
  libprotoc-dev \
  autoconf \
  libbsd-dev \
  libnet-dev \
  psmisc \
  libprotobuf-dev \
  libprotobuf-c-dev \
  protobuf-c-compiler \
  protobuf-compiler \
  python3-protobuf \
  python3-future \
  libnl-3-dev \
  libcap-dev \
  lsb-release \
  wget \
  software-properties-common \
  librdkafka-dev \
  redis \
  zookeeper \
  gcc-multilib \
  gawk \
  gcc \
  g++ \
  libc++-dev \
  liblzma-dev \
  lzma-dev \
  leiningen \
  valgrind \
  libelf1 \
  libelf-dev \
  zlib1g-dev \
  bear \
  capnproto \
  libcapnp-dev \
  g++-multilib \
  cmake \
  tmux \
  curl \
  net-tools \
  uml-utilities \
  openjdk-11-jdk \
  libnfnetlink-dev \
  git \
  libdrm-dev \
  gnutls-dev \
  libnftables-dev \
  asciidoc \
  xmlto \
  python3-pip \
  sudo \
  liblua5.1-dev \
  liblua5.1-0 \
  lua-bitop \
  lua-cjson \
  libtool \
  libuv1-dev \
  liblz4-dev \
  npm \
  coffeescript \
  libcurl4-openssl-dev \
  zlib1g-dev \
  libsqlite3-dev \
  --no-install-recommends -y && \
  apt clean && \
  rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

# Install Node.js 16 for RethinkDB
RUN curl -sL https://deb.nodesource.com/setup_16.x -o setup_16.x && \
  chmod +x setup_16.x && \
  sed -i '/sleep/d' setup_16.x && \
  ./setup_16.x && \
  apt-get install -y nodejs && \
  npm install -g browserify@13.1.0

# Install lein
RUN wget https://raw.githubusercontent.com/technomancy/leiningen/stable/bin/lein && \
  mv lein /usr/bin/lein && \
  chmod +x /usr/bin/lein

# Install aerospike-client-c
RUN wget https://download.aerospike.com/artifacts/aerospike-client-c/5.2.0/aerospike-client-c-5.2.0.ubuntu18.04.x86_64.tgz && \
  tar xvf aerospike-client-c-5.2.0.ubuntu18.04.x86_64.tgz && \
  cd aerospike-client-c-5.2.0.ubuntu18.04.x86_64 && \
  dpkg -i aerospike-client-c-devel-5.2.0.ubuntu18.04.x86_64.deb

# Install criu
RUN wget https://github.com/checkpoint-restore/criu/archive/refs/tags/v4.0.tar.gz && \
  tar xvf v4.0.tar.gz && \
  cd criu-4.0 && \
  make -j$(nproc) && \
  make install

# Create user zyh
RUN useradd -m zyh && \
  echo zyh:zyh | chpasswd && \
  touch /etc/sudoers && \
  cp /etc/sudoers /etc/sudoers.bak && \
  echo 'zyh  ALL=(root) NOPASSWD: ALL' >> /etc/sudoers
USER zyh

WORKDIR /home/zyh

# Build brpc & braft
# brpc & braft: https://github.com/zouyonghao/brpc-test.git https://github.com/zouyonghao/braft-test
RUN git clone https://github.com/zouyonghao/brpc-test.git brpc && \
  cd brpc && \
  sh config_brpc.sh --headers=/usr/include --libs=/usr/lib && \
  CC="clang-9 -fsanitize=address" CXX="clang++-9 -fsanitize=address" make -j$(nproc) && \
  sudo cp -r output/* /usr/local

RUN git clone https://github.com/zouyonghao/braft-test.git braft && \
  cd braft && \
  ./build.sh

# Build AerospikeDB
# AerospikeDB https://github.com/zouyonghao/aerospikedb-test.git
RUN git clone https://github.com/zouyonghao/aerospikedb-test.git aerospike-server && \
  cd aerospike-server && \
  ./build.sh

# Dqlite https://github.com/zouyonghao/dqlite-test.git
## Install golang 1.22.4
RUN wget https://golang.org/dl/go1.14.4.linux-amd64.tar.gz && \
  sudo tar -C /usr/local -xzf go1.14.4.linux-amd64.tar.gz && \
  mkdir -p /home/zyh/go/{bin,pkg,src} && \
  rm go1.14.4.linux-amd64.tar.gz

ENV PATH="/usr/local/go/bin:$PATH"
ENV GOPATH="/home/zyh/go"

RUN git clone https://github.com/zouyonghao/dqlite-test.git dqlite-test && \
  cd dqlite-test && \
  cd raft && \
  autoreconf -i && \
  ./configure --enable-example --prefix=/usr && \
  make -j$(nproc) && \
  sudo make install && \
  cd ../dqlite && \
  autoreconf -i && \
  ./configure --prefix=/usr && \
  make -j$(nproc) && \
  sudo make install && \
  cd ../go-dqlite && \
  go install -tags libsqlite3 ./cmd/dqlite-demo

# Use Python2 to build
RUN sudo update-alternatives --install /usr/bin/python python /usr/bin/python2 10
# RethinkDB https://github.com/zouyonghao/rethinkdb-test.git
RUN git clone https://github.com/zouyonghao/rethinkdb-test.git rethinkdb && \
  cd rethinkdb && \
  ./build.sh

# We use a newer version of etcd because the old version in the paper is buggy and is only used for comparison purpose.
# etcd https://github.com/etcd-io/etcd/releases/download/v3.5.1/etcd-v3.5.1-linux-amd64.tar.gz
RUN wget https://github.com/etcd-io/etcd/releases/download/v3.5.1/etcd-v3.5.1-linux-amd64.tar.gz && \
  tar xvf etcd-v3.5.1-linux-amd64.tar.gz && \
  mv etcd-v3.5.1-linux-amd64 etcd && \
  rm etcd-v3.5.1-linux-amd64.tar.gz

# We use a newer version of ZooKeeper because the old version in the paper is buggy and is only used for comparison purpose.
# ZooKeeper https://archive.apache.org/dist/zookeeper/zookeeper-3.7.0/apache-zookeeper-3.7.0.tar.gz
RUN wget https://archive.apache.org/dist/zookeeper/zookeeper-3.7.0/apache-zookeeper-3.7.0-bin.tar.gz && \
  tar xvf apache-zookeeper-3.7.0-bin.tar.gz && \
  mv apache-zookeeper-3.7.0-bin zookeeper && \
  mv zookeeper/lib/zookeeper-3.7.0.jar zookeeper/lib/zookeeper.jar && \
  rm apache-zookeeper-3.7.0-bin.tar.gz

# Install ClickHouse
RUN wget https://github.com/ClickHouse/ClickHouse/releases/download/v21.9.2.17-stable/clickhouse-common-static_21.9.2.17_amd64.deb && \
  sudo dpkg -i clickhouse-common-static_21.9.2.17_amd64.deb && \
  rm clickhouse-common-static_21.9.2.17_amd64.deb && \
  wget https://github.com/ClickHouse/ClickHouse/releases/download/v21.9.2.17-stable/clickhouse-server_21.9.2.17_all.deb && \
  sudo dpkg -i clickhouse-server_21.9.2.17_all.deb && \
  rm clickhouse-server_21.9.2.17_all.deb

# Install NuRaft
RUN git clone https://github.com/zouyonghao/NuRaft-test.git NuRaft && \
  cd NuRaft && \
  ./build.sh

# Install Redis
RUN git clone https://github.com/zouyonghao/redis-test.git redis-test && \
  cd redis-test && \
  cd redis && \
  ./build.sh && \
  cd ../redisraft && \
  ./build.sh

# Install HDFS
RUN wget https://archive.apache.org/dist/hadoop/common/hadoop-3.2.4/hadoop-3.2.4.tar.gz && \
  tar xvf hadoop-3.2.4.tar.gz && \
  mv hadoop-3.2.4 hadoop && \
  rm hadoop-3.2.4.tar.gz

# Install Knossos
RUN git clone https://github.com/zouyonghao/knossos-test.git knossos && \
  cd knossos && \
  lein run data/cas-register/good/cas-register-bug.edn

# Build DistFuzz
COPY --chown=zyh:zyh . DistFuzz

RUN cd DistFuzz && \
  mkdir build && \
  cd build && \
  cmake .. && \
  make -j$(nproc)

# Build strace
RUN cd DistFuzz/strace && \
  ./bootstrap && \
  ./configure && \
  make -j$(nproc)

# Build rethinkdb client
RUN cd /home/zyh/DistFuzz/rethinkdb_test/client && ./build.sh

RUN sudo chown -R zyh:zyh /var/log/zookeeper

ENV PATH="/home/zyh/DistFuzz/build/rr/bin:/home/zyh/DistFuzz/strace/src:$PATH"
