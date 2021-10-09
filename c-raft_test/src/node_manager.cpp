#include <craft_configuration_generator.hpp>

REGISTER_NODE_MANAGER(CRAFT_NODE_MANAGER, new NodeManager(new CraftConfigurationGenerator()));
