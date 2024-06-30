#include <dqlite_configuration_generator.hpp>

REGISTER_NODE_MANAGER(DQLITE_NODE_MANAGER, new NodeManager(new DqliteServerConfigurationGenerator()));
