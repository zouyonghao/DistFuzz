// #include <operator/dst_default_client_operator.hpp>

// static const std::string NURAFT_CLIENT =
//     "bash /home/zyh/distributed-system-test/nuraft_test/bin/run_client.sh";
// static const std::string UNFINISHED_COMMAND =
//     "timeout 3 " + NURAFT_CLIENT + " ";

// REGISTER_NORMAL_OPERATOR(NuraftTestGet,
//                          new DefaultClientOperator(UNFINISHED_COMMAND + "get",
//                                                    "read", {}));
// REGISTER_NORMAL_OPERATOR(NuraftTestSet,
//                          new DefaultClientOperator(UNFINISHED_COMMAND + "set",
//                                                    "write", {" "}));

// REGISTER_NORMAL_OPERATOR(
//     Init, new DefaultClientOperator("timeout 10 " + NURAFT_CLIENT + " set",
//                                     "write", {" "}));