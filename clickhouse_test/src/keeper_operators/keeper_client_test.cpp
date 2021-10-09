// #include <operator/dst_default_client_operator.hpp>

// static const std::string ZK_CLIENT =
//     "/usr/share/zookeeper/bin/zkCli.sh -server 127.0.1.1:9181,127.0.1.1:9182,127.0.1.1:9183 ";
// static const std::string UNFINISHED_COMMAND = "timeout 5 " + ZK_CLIENT + " ";

// REGISTER_NORMAL_OPERATOR(KeeperGet, new DefaultClientOperator(UNFINISHED_COMMAND + "get /a", "read", {}));
// REGISTER_NORMAL_OPERATOR(KeeperSet, new DefaultClientOperator(UNFINISHED_COMMAND + "set /a", "write", {" "}));
// // REGISTER_NORMAL_OPERATOR(KeeperCas, new DefaultClientOperator(UNFINISHED_COMMAND + "cas ", "cas", {" ", " "}));
// // REGISTER_NORMAL_OPERATOR(Init, new DefaultClientOperator("timeout 30 " + ZK_CLIENT + " " + "create /a null", "read", {}));
