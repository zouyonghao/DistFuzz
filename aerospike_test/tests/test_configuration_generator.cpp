#include <aerospike_configuration_generator.hpp>

void test_configuration_generator()
{
    AerospikeConfigurationGenerator acg;
    assert(acg.get_configure_string(0, 3) == "/home/zyh/aerospike-server/target/Linux-x86_64/bin/asd --config-file aerospike_conf0");

    assert(acg.get_configure_string(1, 5) == "/home/zyh/aerospike-server/target/Linux-x86_64/bin/asd --config-file aerospike_conf1");
}

int main() { test_configuration_generator(); }