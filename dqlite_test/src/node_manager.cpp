#include <dqlite_node_manager.hpp>

REGISTER_NODE_MANAGER(DQLITE_NODE_MANAGER, new DqliteNodeManager(new DqliteServerConfigurationGenerator()));
