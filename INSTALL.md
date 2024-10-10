## Installation
We suggest you run the experiments using the Docker image we provide.
```bash
docker run -it --network none --cap-add=NET_ADMIN --device=/dev/net/tun --cap-add=SYS_PTRACE --cap-add=SYS_NICE --cap-add=IPC_LOCK --security-opt seccomp=unconfined zouyonghao/distfuzz:artifact
```

Or, you can build the Docker container yourself using the Dockerfile included in the repository.
```bash
docker build . -t distfuzz:artifact    # It may take 10 ~ 20 minutes.
docker run -it --network none --cap-add=NET_ADMIN --device=/dev/net/tun --cap-add=SYS_PTRACE --cap-add=SYS_NICE --cap-add=IPC_LOCK --security-opt seccomp=unconfined distfuzz:artifact
```

Alternatively, you can manually install DistFuzz and target systems by following the steps below. Please note that we have have only tested building and running DistFuzz with Ubuntu 20.04.
1. Install the dependencies

    There are many dependencies that are not required by DistFuzz itself but for target systems.

    ```bash
    sudo apt update
    sudo DEBIAN_FRONTEND=noninteractive apt install \
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
        --no-install-recommends -y
    ```

2. Install Node.js 16 for RethinkDB
    ```bash
    curl -sL https://deb.nodesource.com/setup_16.x -o setup_16.x
    chmod +x setup_16.x
    sed -i '/sleep/d' setup_16.x # this is for disabling the sleeping caused by installing an old version of Node.js
    ./setup_16.x # You may need sudo
    sudo apt-get install -y nodejs
    npm install -g browserify@13.1.0 # You may need sudo
    ```

3. Install lein
    ```bash
    wget https://raw.githubusercontent.com/technomancy/leiningen/stable/bin/lein
    sudo mv lein /usr/bin/lein
    sudo chmod +x /usr/bin/lein
    ```

4. Install aerospike-client-c
    ```bash
    wget https://download.aerospike.com/artifacts/aerospike-client-c/5.2.0/aerospike-client-c-5.2.0.ubuntu18.04.x86_64.tgz
    tar xvf aerospike-client-c-5.2.0.ubuntu18.04.x86_64.tgz
    cd aerospike-client-c-5.2.0.ubuntu18.04.x86_64
    dpkg -i aerospike-client-c-devel-5.2.0.ubuntu18.04.x86_64.deb
    ```

5. Install criu
    ```bash
    wget https://github.com/checkpoint-restore/criu/archive/refs/tags/v4.0.tar.gz
    tar xvf v4.0.tar.gz
    cd criu-4.0
    make -j$(nproc)
    sudo make install
    ```

6. Create user zyh

    DistFuzz assumes many operations are done under folder /home/zyh
    ```bash
    useradd -m zyh
    echo zyh:zyh | chpasswd
    touch /etc/sudoers
    cp /etc/sudoers /etc/sudoers.bak
    echo 'zyh  ALL=(root) NOPASSWD: ALL' >> /etc/sudoers
    ```

7. Build DistFuzz

    #### Build DistFuzz

    ```bash
    git clone https://github.com/zouyonghao/DistFuzz.git
    cd DistFuzz
    mkdir build
    cd build
    cmake ..
    make -j$(nproc)
    ```

    #### Build strace
    ```bash
    cd DistFuzz/strace
    ./bootstrap
    ./configure
    make -j$(nproc)
    ```

    #### Install Knossos
    ```bash
    git clone https://github.com/zouyonghao/knossos-test.git knossos
    cd knossos
    lein run data/cas-register/good/cas-register-bug.edn
    ```

8. Build target systems

    Login to user `zyh` and install target systems in the folder `/home/zyh`:
    ```bash
    su zyh
    cd /home/zyh
    ```

    #### Build brpc & braft
    brpc & braft: https://github.com/zouyonghao/brpc-test.git https://github.com/zouyonghao/braft-test
    ```bash
    git clone https://github.com/zouyonghao/brpc-test.git brpc
    cd brpc
    sh config_brpc.sh --headers=/usr/include --libs=/usr/lib
    CC="clang-9 -fsanitize=address" CXX="clang++-9 -fsanitize=address" make -j$(nproc)
    sudo cp -r output/* /usr/local

    cd /home/zyh
    git clone https://github.com/zouyonghao/braft-test.git braft
    cd braft
    ./build.sh
    ```

    #### Build AerospikeDB
    AerospikeDB: https://github.com/zouyonghao/aerospikedb-test.git
    ```bash
    git clone https://github.com/zouyonghao/aerospikedb-test.git aerospike-server
    cd aerospike-server
    ./build.sh
    ```

    #### Build Dqlite
    Dqlite: https://github.com/zouyonghao/dqlite-test.git
    ```bash
    # Install golang 1.22.4
    wget https://golang.org/dl/go1.14.4.linux-amd64.tar.gz
    sudo tar -C /usr/local -xzf go1.14.4.linux-amd64.tar.gz
    mkdir -p /home/zyh/go/{bin,pkg,src}
    rm go1.14.4.linux-amd64.tar.gz
    export PATH="/usr/local/go/bin:$PATH"
    export GOPATH="/home/zyh/go"

    # Build raft, dqlite and go-dqlite
    git clone https://github.com/zouyonghao/dqlite-test.git dqlite-test
    cd dqlite-test
    cd raft
    autoreconf -i
    ./configure --enable-example --prefix=/usr
    make -j$(nproc)
    sudo make install
    cd ../dqlite
    autoreconf -i
    ./configure --prefix=/usr
    make -j$(nproc)
    sudo make install
    cd ../go-dqlite
    go install -tags libsqlite3 ./cmd/dqlite-demo
    ```

    #### RethinkDB
    RethinkDB: https://github.com/zouyonghao/rethinkdb-test.git
    ```
    # Use Python2 to build
    sudo update-alternatives --install /usr/bin/python python /usr/bin/python2 10
    # RethinkDB https://github.com/zouyonghao/rethinkdb-test.git
    git clone https://github.com/zouyonghao/rethinkdb-test.git rethinkdb
    cd rethinkdb
    ./build.sh
    ```

    #### etcd
    We use a newer version of etcd because the old version in the paper is buggy and is only used for comparison purpose.

    etcd https://github.com/etcd-io/etcd/releases/download/v3.5.1/etcd-v3.5.1-linux-amd64.tar.gz

    ```bash
    wget https://github.com/etcd-io/etcd/releases/download/v3.5.1/etcd-v3.5.1-linux-amd64.tar.gz
    tar xvf etcd-v3.5.1-linux-amd64.tar.gz
    mv etcd-v3.5.1-linux-amd64 etcd
    rm etcd-v3.5.1-linux-amd64.tar.gz
    ```

    #### ZooKeeper
    We use a newer version of ZooKeeper because the old version in the paper is buggy and is only used for comparison purpose.

    ZooKeeper https://archive.apache.org/dist/zookeeper/zookeeper-3.7.0/apache-zookeeper-3.7.0.tar.gz

    ```bash
    wget https://archive.apache.org/dist/zookeeper/zookeeper-3.7.0/apache-zookeeper-3.7.0-bin.tar.gz
    tar xvf apache-zookeeper-3.7.0-bin.tar.gz
    mv apache-zookeeper-3.7.0-bin zookeeper
    mv zookeeper/lib/zookeeper-3.7.0.jar zookeeper/lib/zookeeper.jar
    rm apache-zookeeper-3.7.0-bin.tar.gz
    ```


    #### Build rethinkdb client
    ```bash
    cd /home/zyh/DistFuzz/rethinkdb_test/client && ./build.sh
    ```

9. Prepare the environment

    ```bash
    sudo chown -R zyh:zyh /var/log/zookeeper
    export PATH="/home/zyh/DistFuzz/build/rr/bin:/home/zyh/DistFuzz/strace/src:$PATH"
    ```


Once everything is installed, to review our artifact, follow the instructions in `README.md`.
