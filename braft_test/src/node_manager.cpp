#include <dst_node_manager.hpp>

#define BIN_PATH "/home/zyh/braft/example/atomic/atomic_server"

#define BASE_PORT 8300

/* The ConfigurationGenerator for braft, the example is as follows:
 * atomic_server -reuse_addr -ip=127.0.1.1 -reuse_port -port=8300 -election_timeout_ms=200
 * -conf="127.0.1.1:8300:0,127.0.1.1:8301:0,127.0.1.1:8302:0," -data_path="./data0
 */
class BraftConfigurationGenerator : public ConfigurationGenerator
{
    std::string get_configure_string(uint32_t node_id, uint32_t node_count)
    {
        std::string config = BIN_PATH " -reuse_addr -ip=127.0.1.1 -reuse_port -election_timeout_ms=200 ";
        config += "-port=" + std::to_string(BASE_PORT + node_id) + " ";
        config += "-conf=";
        for (uint32_t i = 0; i < node_count; i++)
        {
            config += "127.0.1.1:" + std::to_string(BASE_PORT + i) + ":0,";
        }
        config += " ";
        config += "-data_path=./data" + std::to_string(node_id);
        return config;
    }
};

REGISTER_NODE_MANAGER(BRAFT_NODE_MANAGER, new NodeManager(new BraftConfigurationGenerator()));
