#include <keeper_configuration_generator.hpp>

void test_configuration_generator()
{
    KeeperConfigurationGenerator kcg;
    assert(kcg.get_configure_string(0, 3) ==
           "/home/zyh/ClickHouse/build/programs/clickhouse-keeper --config enable_keeper0.xml");
}

void test_client_configuration_generator()
{
    KeeperClientConfigurationGenerator kccg;
    assert(kccg.get_configure_string(OP_READ, 3) ==
           "timeout 2 /usr/share/zookeeper/bin/zkCli.sh -server 127.0.1.1:9181,127.0.1.1:9182,127.0.1.1:9183 get /a");
}

int main()
{
    test_configuration_generator();
    test_client_configuration_generator();
}