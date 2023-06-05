set -e
sudo apt update
sudo apt-get install \
    libboost-all-dev \
    rapidjson-dev \
    clang \
    libssl-dev \
    libleveldb-dev \
    libgflags-dev \
    libprotobuf-dev \
    libprotoc-dev \
    protobuf-compiler \
    libleveldb-dev \
    autoconf \
    libbsd-dev \
    libnet-dev \
    psmisc \
    libprotobuf-dev libprotobuf-c-dev protobuf-c-compiler protobuf-compiler python3-protobuf python3-future libnl-3-dev libcap-dev \
    lsb-release wget software-properties-common \
    librdkafka-dev \
    redis \
    zookeeper \
    gcc-multilib \
    gawk gcc g++ libc++-dev liblzma-dev lzma-dev leiningen valgrind libelf1 libelf-dev zlib1g-dev bear \
    capnproto libcapnp-dev g++-multilib -y > /dev/null && echo "apt-get dependencies success!"

# NOTE:
# libunwind-dev

cd
# install aerospike client
if ls /usr/include/aerospike > /dev/null; then
    echo "aerospike client is installed, skip."
else
    echo "installing aerospike client..."
    # wget https://download.aerospike.com/artifacts/aerospike-client-c/5.2.0/aerospike-client-c-5.2.0.ubuntu18.04.x86_64.tgz
    # for 22.04
    wget https://download.aerospike.com/artifacts/aerospike-client-c/6.1.1/aerospike-client-c-6.1.1.ubuntu22.04.x86_64.tgz
    tar xvf aerospike-client-c-5.2.0.ubuntu18.04.x86_64.tgz
    cd aerospike-client-c-5.2.0.ubuntu18.04.x86_64
    sudo dpkg -i aerospike-client-c-devel-5.2.0.ubuntu18.04.x86_64.deb
    cd ..
    echo "install aerospike client lib success!"
fi

# install java, lein
# if lein version > /dev/null; then
#     echo "leiningen is installed, skip."
# else
#     echo "installing leiningen and jdk..."
#     wget https://raw.githubusercontent.com/technomancy/leiningen/stable/bin/lein
#     sudo mv lein /usr/bin/lein
#     # sudo mv distributed-system-test/lein /usr/bin/
#     sudo chmod +x /usr/bin/lein
#     sudo apt install default-jdk -y > /dev/null
#     echo "install lein success!"
# fi

# cd
# if ls criu > /dev/null; then
#     echo "it seems you have installed criu, skip."
# else
#     echo "installing criu..."
#     git clone http://166.111.139.110:7800/zyh/criu.git
#     cd criu
#     make && echo "install criu success!"
# fi

# cd
# if clang-11 --version > /dev/null; then
#     echo "clang-11 is installed, skip."
# else
#     echo "installing clang-11..."
#     wget https://apt.llvm.org/llvm.sh
#     chmod +x llvm.sh
#     sudo ./llvm.sh 11
#     echo "install clang and llvm 11 success!"
# fi