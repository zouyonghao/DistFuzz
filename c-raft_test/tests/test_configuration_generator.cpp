#include <craft_configuration_generator.hpp>

void test_configuration_generator()
{
    CraftConfigurationGenerator ccg;
    assert(ccg.get_configure_string(0, 3) == "/home/zyh/raft/example/server ./c-raft_data0 1 9000 9000 9001 9002 ");
}

int main() { test_configuration_generator(); }