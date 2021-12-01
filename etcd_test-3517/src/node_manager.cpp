#include <dst_node_manager.hpp>
#include <etcd_configuration_generator.hpp>

REGISTER_NODE_MANAGER(ETCD_NODE_MANAGER, new NodeManager(new EtcdConfigurationGenerator));
