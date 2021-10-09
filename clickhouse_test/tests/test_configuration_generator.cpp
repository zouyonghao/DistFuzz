#include <keeper_configuration_generator.hpp>

void test_configuration_generator()
{
    KeeperConfigurationGenerator kcg;
    assert(kcg.get_configure_string(0, 3) == "/home/zyh/ClickHouse/build/programs/clickhouse-keeper --config enable_keeper0.xml");
}

int main() { test_configuration_generator(); }