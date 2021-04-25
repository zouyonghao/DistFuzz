#include <braft_test_common.hpp>

#include <operator/dst_kill_by_name_operator.hpp>

REGISTER_CRITICAL_OPERATOR(KillNode0, new KillByName("node0", "8300", false));
REGISTER_CRITICAL_OPERATOR(KillNode1, new KillByName("node1", "8301", false));
REGISTER_CRITICAL_OPERATOR(KillNode2, new KillByName("node2", "8303", false));

REGISTER_CRITICAL_OPERATOR(ForceKillNode0,
                           new KillByName("node0", "8300", true));
REGISTER_CRITICAL_OPERATOR(ForceKillNode1,
                           new KillByName("node1", "8301", true));
REGISTER_CRITICAL_OPERATOR(ForceKillNode2,
                           new KillByName("node2", "8303", true));