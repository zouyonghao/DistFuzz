#include <operator/dst_default_client_operator.hpp>

static const std::string ATOMIC_TEST =
    "/home/zyh/braft/example/atomic/atomic_test";
static const std::string ATOMIC_CONF =
    "127.0.1.1:8300:0,127.0.1.1:8301:0,127.0.1.1:8302:0,";
static const std::string UNFINISHED_COMMAND =
    "timeout 3 " + ATOMIC_TEST + " -conf=" + ATOMIC_CONF + " -atomic_op=";
// static const std::string UNFINISHED_COMMAND =
//     ATOMIC_TEST + " -conf=" + ATOMIC_CONF + " -atomic_op=";

// ./atomic_test -conf="127.0.1.1:8300:0,127.0.1.1:8301:0,127.0.1.1:8302:0,"
// -atomic_op="cas" -atomic_id=1 -atomic_val=10 -atomic_new_val=12
REGISTER_NORMAL_OPERATOR(AtomicTestGet,
                         new DefaultClientOperator(UNFINISHED_COMMAND + "get",
                                                   "read", {}));
REGISTER_NORMAL_OPERATOR(AtomicTestSet,
                         new DefaultClientOperator(UNFINISHED_COMMAND + "set",
                                                   "write", {" -atomic_val="}));
REGISTER_NORMAL_OPERATOR(
    AtomicTestCas,
    new DefaultClientOperator(UNFINISHED_COMMAND + "cas", "cas",
                              {" -atomic_val=", " -atomic_new_val="}));

REGISTER_NORMAL_OPERATOR(
    Init, new DefaultClientOperator("timeout 10 " + ATOMIC_TEST +
                                        " -conf=" + ATOMIC_CONF +
                                        " -atomic_op=set",
                                    "write", {" -atomic_val="}));