#include <operator/dst_kill_by_name_operator.hpp>

REGISTER_CRITICAL_OPERATOR(KillServer0,
                           new KillByName("node0", "c-raft_data0", false));
REGISTER_CRITICAL_OPERATOR(KillServer1,
                           new KillByName("node1", "c-raft_data1", false));
REGISTER_CRITICAL_OPERATOR(KillServer2,
                           new KillByName("node2", "c-raft_data2", false));

REGISTER_CRITICAL_OPERATOR(ForceKillServer0,
                           new KillByName("node0", "c-raft_data0", true));
REGISTER_CRITICAL_OPERATOR(ForceKillServer1,
                           new KillByName("node1", "c-raft_data1", true));
REGISTER_CRITICAL_OPERATOR(ForceKillServer2,
                           new KillByName("node2", "c-raft_data2", true));