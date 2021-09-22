set -e
sudo apt-get install \
    libboost-all-dev \
    rapidjson-dev \
    clang-9 \
    libssl-dev \
    libleveldb-dev \
    libgflags-dev \
    libprotobuf-dev \
    libprotoc-dev \
    protobuf-compiler \
    libleveldb-dev \
    autoconf \
    libbsd-dev \
    libprotobuf-dev libprotobuf-c-dev protobuf-c-compiler protobuf-compiler python3-protobuf python3-future libnl-3-dev libcap-dev \
    redis -y > /dev/null && echo "apt-get dependencies success!"

# install aerospike client
if ls /usr/include/aerospike > /dev/null; then
    echo "aerospike client is installed, skip."
else
    echo "installing aerospike client..."
    wget https://download.aerospike.com/artifacts/aerospike-client-c/5.2.0/aerospike-client-c-5.2.0.ubuntu18.04.x86_64.tgz
    tar xvf aerospike-client-c-5.2.0.ubuntu18.04.x86_64.tgz
    cd aerospike-client-c-5.2.0.ubuntu18.04.x86_64
    sudo dpkg -i aerospike-client-c-devel-5.2.0.ubuntu18.04.x86_64.deb
    cd ..
    echo "install aerospike client lib success!"
fi

# install java, lein
if lein version > /dev/null; then
    echo "leiningen is installed, skip."
else
    echo "installing leiningen and jdk..."
    wget https://raw.githubusercontent.com/technomancy/leiningen/stable/bin/lein
    sudo mv lein /usr/bin/
    sudo chmod +x /usr/bin/lein
    sudo apt install default-jdk
    echo "install lein success!"
fi

cd
if ls criu > /dev/null; then
    echo "it seems you have installed criu, skip."
else
    echo "installing criu..."
    git clone http://166.111.139.139:7800/zyh/criu.git
    cd criu
    make && echo "install criu success!"
fi
