#include <aerospike_configuration_generator.hpp>
#include <dst_node_manager.hpp>

class AerospikeNodeManager : public NodeManager
{
public:
    AerospikeNodeManager(ServerConfigurationGenerator *scg) : NodeManager(scg) {}
    bool start_all() override
    {
        bool result = NodeManager::start_all();
        sleep(2);
        return result;
    }
};

REGISTER_NODE_MANAGER(AEROSPIKE_NODE_MANAGER, new AerospikeNodeManager(new AerospikeConfigurationGenerator()));
