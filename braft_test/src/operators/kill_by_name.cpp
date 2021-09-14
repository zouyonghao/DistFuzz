#include <braft_test_common.hpp>

#include <operator/dst_kill_by_name_operator.hpp>

REGISTER_CRITICAL_OPERATOR(KillNode0, new KillByName("node0", "data0", false));
REGISTER_CRITICAL_OPERATOR(KillNode1, new KillByName("node1", "data1", false));
REGISTER_CRITICAL_OPERATOR(KillNode2, new KillByName("node2", "data2", false));

REGISTER_CRITICAL_OPERATOR(ForceKillNode0,
                           new KillByName("node0", "data0", true));
REGISTER_CRITICAL_OPERATOR(ForceKillNode1,
                           new KillByName("node1", "data1", true));
REGISTER_CRITICAL_OPERATOR(ForceKillNode2,
                           new KillByName("node2", "data2", true));