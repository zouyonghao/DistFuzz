#include <operator/dst_default_client_operator.hpp>

static const std::string RAFTIS_CLIENT =
    "bash /root/distributed-system-test/raftis_test/bin/run_client.sh";
static const std::string UNFINISHED_COMMAND =
    "timeout 3 " + RAFTIS_CLIENT + " ";

REGISTER_NORMAL_OPERATOR(RaftisTestGet,
                         new DefaultClientOperator(UNFINISHED_COMMAND + "get",
                                                   "read", {}));
REGISTER_NORMAL_OPERATOR(RaftisTestSet,
                         new DefaultClientOperator(UNFINISHED_COMMAND + "set",
                                                   "write", {" "}));

REGISTER_NORMAL_OPERATOR(
    Init, new DefaultClientOperator("timeout 10 " + RAFTIS_CLIENT + " set",
                                    "write", {" "}));