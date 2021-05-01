#include <operator/dst_start_by_name_operator.hpp>

REGISTER_CRITICAL_OPERATOR(StartServer0, new StartByName("node0", "./start_server0.sh"));
REGISTER_CRITICAL_OPERATOR(StartServer1, new StartByName("node1", "./start_server1.sh"));
REGISTER_CRITICAL_OPERATOR(StartServer2, new StartByName("node2", "./start_server2.sh"));