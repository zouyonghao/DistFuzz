#include <keeper_configuration_generator.hpp>

REGISTER_NODE_MANAGER(KEEPER_NODE_MANAGER, new NodeManager(new KeeperConfigurationGenerator()));
