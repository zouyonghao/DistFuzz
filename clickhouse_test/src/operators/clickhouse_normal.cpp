#include <operator/dst_default_client_operator.hpp>

// static const std::string CLICKHOUSE_CLIENT =
//     "/home/zyh/ClickHouse/build/programs/clickhouse-client -c";

// static const std::string UNFINISHED_COMMAND =
//     "timeout 2 " + CLICKHOUSE_CLIENT + " ";

// REGISTER_NORMAL_OPERATOR(ClickHouseInsert0,
//                          new DefaultClientOperator(UNFINISHED_COMMAND +
//                                                        "config0.xml -q \"insert into test.test values",
//                                                    "", {"(", ",", ");\""}));
// REGISTER_NORMAL_OPERATOR(ClickHouseInsert1,
//                          new DefaultClientOperator(UNFINISHED_COMMAND +
//                                                        "config1.xml -q \"insert into test.test values",
//                                                    "", {"(", ",", ");\""}));
// REGISTER_NORMAL_OPERATOR(ClickHouseInsert2,
//                          new DefaultClientOperator(UNFINISHED_COMMAND +
//                                                        "config2.xml -q \"insert into test.test values",
//                                                    "", {"(", ",", ");\""}));

// REGISTER_NORMAL_OPERATOR(ClickHouseSelect0,
//                          new DefaultClientOperator(UNFINISHED_COMMAND +
//                                                        "config0.xml -q \"select * from test.test where key=",
//                                                    "", {"\""}));
// REGISTER_NORMAL_OPERATOR(ClickHouseSelect1,
//                          new DefaultClientOperator(UNFINISHED_COMMAND +
//                                                        "config1.xml -q \"select * from test.test where key=",
//                                                    "", {"\""}));
// REGISTER_NORMAL_OPERATOR(ClickHouseSelect2,
//                          new DefaultClientOperator(UNFINISHED_COMMAND +
//                                                        "config2.xml -q \"select * from test.test where key=",
//                                                    "", {"\""}));


REGISTER_NORMAL_OPERATOR(ClickHouseInsert0,
                         new DefaultClientOperator("bash client_insert.sh config0.xml ",
                                                   "", {"", ""}));
REGISTER_NORMAL_OPERATOR(ClickHouseInsert1,
                         new DefaultClientOperator("bash client_insert.sh config1.xml ",
                                                   "", {"", ""}));
REGISTER_NORMAL_OPERATOR(ClickHouseInsert2,
                         new DefaultClientOperator("bash client_insert.sh config2.xml ",
                                                   "", {"", ""}));

REGISTER_NORMAL_OPERATOR(ClickHouseSelect0,
                         new DefaultClientOperator("bash client_select.sh config0.xml ",
                                                   "", {""}));
REGISTER_NORMAL_OPERATOR(ClickHouseSelect1,
                         new DefaultClientOperator("bash client_select.sh config1.xml ",
                                                   "", {""}));
REGISTER_NORMAL_OPERATOR(ClickHouseSelect2,
                         new DefaultClientOperator("bash client_select.sh config2.xml ",
                                                   "", {""}));