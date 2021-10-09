// #include <operator/dst_default_client_operator.hpp>

// static const std::string REDISRAFT_CLIENT =
//     "bash /home/zyh/distributed-system-test/redisraft_test/bin/run_client.sh";
// static const std::string UNFINISHED_COMMAND =
//     "timeout 3 " + REDISRAFT_CLIENT + " ";

// REGISTER_NORMAL_OPERATOR(RedisraftTestGet,
//                          new DefaultClientOperator(UNFINISHED_COMMAND + "get",
//                                                    "read", {}));
// REGISTER_NORMAL_OPERATOR(RedisraftTestSet,
//                          new DefaultClientOperator(UNFINISHED_COMMAND + "set",
//                                                    "write", {" "}));

// REGISTER_NORMAL_OPERATOR(
//     Init, new DefaultClientOperator("timeout 10 " + REDISRAFT_CLIENT + " set",
//                                     "write", {" "}));