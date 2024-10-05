#include <operator/dst_kill_by_name_operator.hpp>

REGISTER_CRITICAL_OPERATOR(KillNode0, new KillByName("node0", "config0.xml", false));
REGISTER_CRITICAL_OPERATOR(KillNode1, new KillByName("node1", "config1.xml", false));
REGISTER_CRITICAL_OPERATOR(KillNode2, new KillByName("node2", "config2.xml", false));

REGISTER_CRITICAL_OPERATOR(ForceKillNode0,
                           new KillByName("node0", "config0.xml", true));
REGISTER_CRITICAL_OPERATOR(ForceKillNode1,
                           new KillByName("node1", "config1.xml", true));
REGISTER_CRITICAL_OPERATOR(ForceKillNode2,
                           new KillByName("node2", "config2.xml", true));