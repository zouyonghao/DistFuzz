#include <operator/dst_default_client_operator.hpp>

static const std::string RETHINKDB_CLIENT =
    "java -jar /home/zyh/distributed-system-test/rethinkdb_test/client/target/rethinkdb-test-client-1.0-SNAPSHOT-jar-with-dependencies.jar";
static const std::string UNFINISHED_COMMAND =
    "timeout 5 " + RETHINKDB_CLIENT + " ";

REGISTER_OPERATOR(RethinkDBGet,
                  new DefaultClientOperator(UNFINISHED_COMMAND + "get ", "read",
                                         {}));
REGISTER_OPERATOR(RethinkDBSet,
                  new DefaultClientOperator(UNFINISHED_COMMAND + "set ", "write",
                                         {" "}));
REGISTER_OPERATOR(RethinkDBCas,
                  new DefaultClientOperator(UNFINISHED_COMMAND + "cas ", "cas",
                                         {" ", " "}));