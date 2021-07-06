#include <clickhouse_common_operator.hpp>

// static const std::string CLICKHOUSE_CLIENT =
//     "/home/zyh/ClickHouse/build/programs/clickhouse-client -c";

// static const std::string UNFINISHED_COMMAND =
//     "timeout 2 " + CLICKHOUSE_CLIENT + " ";

// REGISTER_NORMAL_OPERATOR(ClickHouseInsert0,
//                          new ClickHouseCommonOperator(UNFINISHED_COMMAND +
//                                                        "config0.xml -q \"insert into test.test values",
//                                                    {"(", ",", ");\""}));
// REGISTER_NORMAL_OPERATOR(ClickHouseInsert1,
//                          new ClickHouseCommonOperator(UNFINISHED_COMMAND +
//                                                        "config1.xml -q \"insert into test.test values",
//                                                    {"(", ",", ");\""}));
// REGISTER_NORMAL_OPERATOR(ClickHouseInsert2,
//                          new ClickHouseCommonOperator(UNFINISHED_COMMAND +
//                                                        "config2.xml -q \"insert into test.test values",
//                                                    {"(", ",", ");\""}));

// REGISTER_NORMAL_OPERATOR(ClickHouseSelect0,
//                          new ClickHouseCommonOperator(UNFINISHED_COMMAND +
//                                                        "config0.xml -q \"select * from test.test where key=",
//                                                    {"\""}));
// REGISTER_NORMAL_OPERATOR(ClickHouseSelect1,
//                          new ClickHouseCommonOperator(UNFINISHED_COMMAND +
//                                                        "config1.xml -q \"select * from test.test where key=",
//                                                    {"\""}));
// REGISTER_NORMAL_OPERATOR(ClickHouseSelect2,
//                          new ClickHouseCommonOperator(UNFINISHED_COMMAND +
//                                                        "config2.xml -q \"select * from test.test where key=",
//                                                    {"\""}));

REGISTER_NORMAL_OPERATOR(ClickHouseInsert0,
                         new ClickHouseCommonOperator("bash client_insert.sh config0.xml ", {"", ""}));
REGISTER_NORMAL_OPERATOR(ClickHouseInsert1,
                         new ClickHouseCommonOperator("bash client_insert.sh config1.xml ", {"", ""}));
REGISTER_NORMAL_OPERATOR(ClickHouseInsert2,
                         new ClickHouseCommonOperator("bash client_insert.sh config2.xml ", {"", ""}));

REGISTER_NORMAL_OPERATOR(ClickHouseSelect0, new ClickHouseCommonOperator("bash client_select.sh config0.xml ", {""}));
REGISTER_NORMAL_OPERATOR(ClickHouseSelect1, new ClickHouseCommonOperator("bash client_select.sh config1.xml ", {""}));
REGISTER_NORMAL_OPERATOR(ClickHouseSelect2, new ClickHouseCommonOperator("bash client_select.sh config2.xml ", {""}));

REGISTER_NORMAL_OPERATOR(ClickHouseAlter0,
                         new ClickHouseCommonOperator("bash client_alter_test.sh config0.xml ", {"", ""}));
REGISTER_NORMAL_OPERATOR(ClickHouseAlter1,
                         new ClickHouseCommonOperator("bash client_alter_test.sh config1.xml ", {"", ""}));
REGISTER_NORMAL_OPERATOR(ClickHouseAlter2,
                         new ClickHouseCommonOperator("bash client_alter_test.sh config2.xml ", {"", ""}));

REGISTER_NORMAL_OPERATOR(ClickHouseCreateReplicatedDB0,
                         new ClickHouseCommonOperator("bash client_create_db_replicated.sh config0.xml ",
                                                      {"", "", ""}));
REGISTER_NORMAL_OPERATOR(ClickHouseCreateReplicatedDB1,
                         new ClickHouseCommonOperator("bash client_create_db_replicated.sh config1.xml ",
                                                      {"", "", ""}));
REGISTER_NORMAL_OPERATOR(ClickHouseCreateReplicatedDB2,
                         new ClickHouseCommonOperator("bash client_create_db_replicated.sh config2.xml ",
                                                      {"", "", ""}));

REGISTER_NORMAL_OPERATOR(ClickHouseCreateDB0,
                         new ClickHouseCommonOperator("bash client_create_db.sh config0.xml ", {""}));
REGISTER_NORMAL_OPERATOR(ClickHouseCreateDB1,
                         new ClickHouseCommonOperator("bash client_create_db.sh config1.xml ", {""}));
REGISTER_NORMAL_OPERATOR(ClickHouseCreateDB2,
                         new ClickHouseCommonOperator("bash client_create_db.sh config2.xml ", {""}));

REGISTER_NORMAL_OPERATOR(ClickHouseCreateTableRandom0,
                         new ClickHouseCommonOperator("bash client_create_table_random.sh config0.xml ",
                                                      {"", "", "", "", "", "", ""}));
REGISTER_NORMAL_OPERATOR(ClickHouseCreateTableRandom1,
                         new ClickHouseCommonOperator("bash client_create_table_random.sh config1.xml ",
                                                      {"", "", "", "", "", "", ""}));
REGISTER_NORMAL_OPERATOR(ClickHouseCreateTableRandom2,
                         new ClickHouseCommonOperator("bash client_create_table_random.sh config2.xml ",
                                                      {"", "", "", "", "", "", ""}));

REGISTER_NORMAL_OPERATOR(ClickHouseCreateTable0,
                         new ClickHouseCommonOperator("bash client_create_table.sh config0.xml ",
                                                      {"", "", "", "", "", ""}));
REGISTER_NORMAL_OPERATOR(ClickHouseCreateTable1,
                         new ClickHouseCommonOperator("bash client_create_table.sh config1.xml ",
                                                      {"", "", "", "", "", ""}));
REGISTER_NORMAL_OPERATOR(ClickHouseCreateTable2,
                         new ClickHouseCommonOperator("bash client_create_table.sh config2.xml ",
                                                      {"", "", "", "", "", ""}));

REGISTER_NORMAL_OPERATOR(ClickHouseDropTable0, new ClickHouseCommonOperator("bash client_drop.sh config0.xml ", {""}));
REGISTER_NORMAL_OPERATOR(ClickHouseDropTable1, new ClickHouseCommonOperator("bash client_drop.sh config1.xml ", {""}));
REGISTER_NORMAL_OPERATOR(ClickHouseDropTable2, new ClickHouseCommonOperator("bash client_drop.sh config2.xml ", {""}));

REGISTER_NORMAL_OPERATOR(ClickHouseInsertRandom0,
                         new ClickHouseCommonOperator("bash client_insert_random.sh config0.xml ", {"", "", ""}));
REGISTER_NORMAL_OPERATOR(ClickHouseInsertRandom1,
                         new ClickHouseCommonOperator("bash client_insert_random.sh config1.xml ", {"", "", ""}));
REGISTER_NORMAL_OPERATOR(ClickHouseInsertRandom2,
                         new ClickHouseCommonOperator("bash client_insert_random.sh config2.xml ", {"", "", ""}));

REGISTER_NORMAL_OPERATOR(ClickHouseSelectRandom0,
                         new ClickHouseCommonOperator("bash client_select_random.sh config0.xml ", {"", ""}));
REGISTER_NORMAL_OPERATOR(ClickHouseSelectRandom1,
                         new ClickHouseCommonOperator("bash client_select_random.sh config1.xml ", {"", ""}));
REGISTER_NORMAL_OPERATOR(ClickHouseSelectRandom2,
                         new ClickHouseCommonOperator("bash client_select_random.sh config2.xml ", {"", ""}));

REGISTER_NORMAL_OPERATOR(ClearZookeeperData, new ClickHouseCommonOperator("bash clear_zk_data", {}));