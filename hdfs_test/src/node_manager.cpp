#include <dst_node_manager.hpp>
#include <hdfs_configuration_generator.hpp>

REGISTER_NODE_MANAGER(HDFS_NODE_MANAGER, new NodeManager(new HdfsConfigurationGenerator()));
