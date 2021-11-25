#ifndef CRAFT_CONFIGURATION_GENERATOR_HEADER
#define CRAFT_CONFIGURATION_GENERATOR_HEADER

#include <dst_node_manager.hpp>

#define BIN_PATH "/home/zyh/raft/example/server"
#define BASE_PORT 9000

/* The ServerConfigurationGenerator for craft, the example is as follows:
 * server ./c-raft_data0 1 9000 9000 9001 9002
 */
class CraftConfigurationGenerator : public ServerConfigurationGenerator
{
public:
    void init_data_dir(uint32_t node_id) { system(("mkdir -p ./c-raft_data" + std::to_string(node_id)).c_str()); }
    std::string get_configure_string(uint32_t node_id, uint32_t node_count) override
    {
        std::string config = BIN_PATH " ";
        config += "./c-raft_data" + std::to_string(node_id) + " ";
        config += std::to_string(node_id + 1) + " ";
        config += std::to_string(BASE_PORT + node_id) + " ";
        for (uint32_t i = 0; i < node_count; i++)
        {
            config += std::to_string(BASE_PORT + i) + " ";
        }
        /** TODO: should we do this operation here? */
        init_data_dir(node_id);
        return config;
    }

    std::string get_no_fault_files(uint32_t node_id) override { return ".probe"; }
};

#endif // CRAFT_CONFIGURATION_GENERATOR_HEADER