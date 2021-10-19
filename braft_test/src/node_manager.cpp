#include <braft_test_common.hpp>
#include <dst_node_manager.hpp>

REGISTER_NODE_MANAGER(BRAFT_NODE_MANAGER, new NodeManager(new BraftConfigurationGenerator()));
