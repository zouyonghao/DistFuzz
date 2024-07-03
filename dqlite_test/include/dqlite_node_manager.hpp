#ifndef DQLITE_NODE_MANAGER_HEADER
#define DQLITE_NODE_MANAGER_HEADER

#include <dqlite_configuration_generator.hpp>

class DqliteNodeManager : public NodeManager
{
public:
    DqliteNodeManager(ServerConfigurationGenerator *_configuration_generator) : NodeManager(_configuration_generator) {}

    bool grace_stop_node(uint32_t i)
    {
        if (i > node_count - 1)
        {
            LOG_ERROR << "the index " << i << " too large, we only have " << node_count << " nodes.\n";
            return false;
        }
        NodeInfo &ni = node_processes[i];
        if (!ni.should_alive)
        {
            LOG_ERROR << "the node " << i << " is alrealy stoped.\n";
            return true;
        }

        kill(ni.process->id(), SIGTERM);
        ni.process->wait_for(std::chrono::milliseconds(200));

        ni.should_alive = false;
        return true;
    }
};

#endif