#include <operator/dst_kill_by_name_operator.hpp>

REGISTER_CRITICAL_OPERATOR(KillDB0,
                           new KillByName("db0", "rethinkdb_data0", false));
REGISTER_CRITICAL_OPERATOR(KillDB1,
                           new KillByName("db1", "rethinkdb_data1", false));
REGISTER_CRITICAL_OPERATOR(KillDB2,
                           new KillByName("db2", "rethinkdb_data2", false));

REGISTER_CRITICAL_OPERATOR(ForceKillDB0,
                           new KillByName("db0", "rethinkdb_data0", true));
REGISTER_CRITICAL_OPERATOR(ForceKillDB1,
                           new KillByName("db1", "rethinkdb_data1", true));
REGISTER_CRITICAL_OPERATOR(ForceKillDB2,
                           new KillByName("db2", "rethinkdb_data2", true));