#include <redisraft_configuration_generator.hpp>
#include <dst_node_manager.hpp>

REGISTER_NODE_MANAGER(REDISRAFT_NODE_MANAGER, new NodeManager(new RedisraftConfigurationGenerator()));
