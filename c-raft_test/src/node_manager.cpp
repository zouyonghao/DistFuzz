#include <craft_configuration_generator.hpp>
#include <craft_node_manager.hpp>

REGISTER_NODE_MANAGER(CRAFT_NODE_MANAGER, new CraftNodeManager(new CraftConfigurationGenerator()));
