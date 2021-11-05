#include <zookeeper_configuration_generator.hpp>

void test_configuration_generator()
{
    ZooKeeperConfigurationGenerator zkcg;
    assert(zkcg.get_configure_string(0, 3) ==
           "java -cp /home/zyh/zookeeper/lib/zookeeper.jar:/home/zyh/zookeeper/lib/*:/home/zyh/zookeeper/conf "
           "org.apache.zookeeper.server.quorum.QuorumPeerMain zoo1");
}

void test_client_configuration_generator()
{
    ZooKeeperClientConfigurationGenerator zkccg;
    assert(zkccg.get_configure_string(OP_READ, 3) ==
           "timeout 3 /usr/share/zookeeper/bin/zkCli.sh -server 127.0.1.1:2181,127.0.1.1:2182,127.0.1.1:2183 get /a");
    assert(
        zkccg.get_configure_string(OP_WRITE, 3, 100) ==
        "timeout 3 /usr/share/zookeeper/bin/zkCli.sh -server 127.0.1.1:2181,127.0.1.1:2182,127.0.1.1:2183 set /a 100");

    ZooKeeperClientWriteWithVersionConfigurationGenerator zkcwvcg;
    assert(zkcwvcg.get_configure_string(OP_WRITE, 3, 100) ==
           "timeout 3 /usr/share/zookeeper/bin/zkCli.sh -server 127.0.1.1:2181,127.0.1.1:2182,127.0.1.1:2183 set /a "
           "100 0");
}

int main()
{
    test_configuration_generator();
    test_client_configuration_generator();
}