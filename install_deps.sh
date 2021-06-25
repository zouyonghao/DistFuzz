sudo apt install libboost-all-dev rapidjson-dev clang-9 libssl-dev libleveldb-dev libgflags-dev libprotobuf-dev libprotoc-dev protobuf-compiler libleveldb-dev

# install aerospike client
wget https://download.aerospike.com/artifacts/aerospike-client-c/5.2.0/aerospike-client-c-5.2.0.ubuntu18.04.x86_64.tgz
tar xvf aerospike-client-c-5.2.0.ubuntu18.04.x86_64.tgz
cd aerospike-client-c-5.2.0.ubuntu18.04.x86_64
sudo dpkg -i aerospike-client-c-devel-5.2.0.ubuntu18.04.x86_64.deb
cd ..

# install java, lein
wget https://raw.githubusercontent.com/technomancy/leiningen/stable/bin/lein
sudo mv lein /usr/bin/
sudo chmod +x /usr/bin/lein
sudo apt install default-jdk
