#include <dst_node_manager.hpp>
#include <zookeeper_configuration_generator.hpp>

REGISTER_NODE_MANAGER(REDISRAFT_NODE_MANAGER, new NodeManager(new ZooKeeperConfigurationGenerator));
