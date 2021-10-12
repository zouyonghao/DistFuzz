#include <nuraft_configuration_generator.hpp>

void test_configuration_generator()
{
    NuraftConfigurationGenerator ncg;
    assert(ncg.get_configure_string(0, 3) == "/home/zyh/NuRaft/build/examples/calc_server 1 127.0.1.1:10001");
}

void test_client_configuration_generator()
{
    NuraftClientConfigurationGenerator nccg;
    assert(nccg.get_configure_string(OP_READ, 3) ==
           "timeout 2 bash /home/zyh/distributed-system-test/nuraft_test/bin/run_client.sh get");
}

void test_system()
{
    system("echo 123 > abc");
}

int main()
{
    test_configuration_generator();
    test_client_configuration_generator();
    test_system();
}