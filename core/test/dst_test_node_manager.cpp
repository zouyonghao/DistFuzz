#include <dst_node_manager.hpp>

#include <iostream>

class EchoConfigurationGenerator : public ConfigurationGenerator
{
    std::string get_configure_string(uint32_t node_id, uint32_t node_count)
    {
        return "echo node id is " + std::to_string(node_id);
    }
};

int main(int argc, char const *argv[])
{
    NodeManager nm(new EchoConfigurationGenerator());
    nm.set_node_count(10);
    nm.start_all();
    nm.wait_all();
    // system("rm ./log*");
    return 0;
}
