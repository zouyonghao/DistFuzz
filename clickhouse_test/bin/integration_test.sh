set -x
CLIENT=/home/zyh/ClickHouse/build/programs/clickhouse-client

${CLIENT} -c $1 -q "CREATE DATABASE testdb ENGINE = Replicated('/clickhouse/databases/test1', 'shard1', 'replica1');"
${CLIENT} -c $1 -q "CREATE TABLE testdb.replicated_table (d Date, k UInt64, i32 Int32) ENGINE=ReplicatedMergeTree('/test/tmp', 'r') ORDER BY k PARTITION BY toYYYYMM(d);"
${CLIENT} -c $1 -q "CREATE TABLE testdb.replicated_table (d Date, k UInt64, i32 Int32) ENGINE=ReplicatedMergeTree('/test/tmp', 'r', d, k, 8192);"
${CLIENT} -c $1 -q "CREATE TABLE testdb.replicated_table (d Date, k UInt64, i32 Int32) ENGINE=ReplicatedMergeTree('/test/tmp/{shard}', '{replica}', d, k, 8192);"
${CLIENT} -c $1 -q "CREATE TABLE testdb.replicated_table (d Date, k UInt64, i32 Int32) ENGINE=ReplicatedMergeTree ORDER BY k PARTITION BY toYYYYMM(d);"
${CLIENT} -c $1 -q "CREATE TABLE testdb.replicated_table(d Date,k UInt64,i32 Int32)"\
"ENGINE = ReplicatedMergeTree('/clickhouse/tables/uuid/{shard}', '{replica}')"\
"PARTITION BY toYYYYMM(d) ORDER BY kSETTINGS index_granularity = 8192"

${CLIENT} -c $1 -q "show create testdb.replicated_table"

engine=ReplicatedMergeTree
name=testdb.alter_test_$engine

${CLIENT} -c $1 -q "CREATE TABLE $name "\
"(CounterID UInt32, StartDate Date, UserID UInt32, VisitID UInt32, NestedColumn Nested(A UInt8, S String), ToDrop UInt32) "\
"ENGINE = $engine PARTITION BY StartDate ORDER BY (CounterID, StartDate, intHash32(UserID), VisitID);"

${CLIENT} -c $1 -q "ALTER TABLE $name ADD COLUMN Added0 UInt32;"
${CLIENT} -c $1 -q "ALTER TABLE $name ADD COLUMN Added2 UInt32;"
${CLIENT} -c $1 -q "ALTER TABLE $name ADD COLUMN Added1 UInt32 AFTER Added0;"
${CLIENT} -c $1 -q "ALTER TABLE $name ADD COLUMN AddedNested1 Nested(A UInt32, B UInt64) AFTER Added2;"
${CLIENT} -c $1 -q "ALTER TABLE $name ADD COLUMN AddedNested1.C Array(String) AFTER AddedNested1.B;"
${CLIENT} -c $1 -q "ALTER TABLE $name ADD COLUMN AddedNested2 Nested(A UInt32, B UInt64) AFTER AddedNested1;"

full_engine="$engine('/clickhouse/tables/uuid/{shard}', '{replica}')"
${CLIENT} -c $1 -q "CREATE TABLE $name(    CounterID UInt32,    StartDate Date,    UserID UInt32,"\
"    VisitID UInt32,    NestedColumn.A Array(UInt8),    NestedColumn.S Array(String),"\
"    ToDrop UInt32,    Added0 UInt32,    Added1 UInt32,    Added2 UInt32,"\
"    AddedNested1.A Array(UInt32),    AddedNested1.B Array(UInt64),    AddedNested1.C Array(String),"\
"    AddedNested2.A Array(UInt32),    AddedNested2.B Array(UInt64))"\
"ENGINE = $full_enginePARTITION BY StartDateORDER BY (CounterID, StartDate, intHash32(UserID), VisitID)"\
"SETTINGS index_granularity = 8192"

${CLIENT} -c $1 -q "CREATE DATABASE IF NOT EXISTS testdb ENGINE = Replicated('/clickhouse/databases/test1', 'shard1', 'replica3');"

${CLIENT} -c $1 -q "ALTER TABLE $name ADD COLUMN Added3 UInt32;"
${CLIENT} -c $1 -q "ALTER TABLE $name DROP COLUMN AddedNested1;"
${CLIENT} -c $1 -q "ALTER TABLE $name RENAME COLUMN Added1 TO AddedNested1;"

full_engine=engine"('/clickhouse/tables/uuid/{shard}', '{replica}')"
${CLIENT} -c $1 -q "CREATE TABLE $name(    CounterID UInt32,    StartDate Date,    UserID UInt32,"\
"    VisitID UInt32,    NestedColumn.A Array(UInt8),    NestedColumn.S Array(String),"\
"    ToDrop UInt32,    Added0 UInt32,    AddedNested1 UInt32,    Added2 UInt32,"\
"    AddedNested2.A Array(UInt32),    AddedNested2.B Array(UInt64),    Added3 UInt32)"\
"ENGINE = $full_enginePARTITION BY StartDateORDER BY (CounterID, StartDate, intHash32(UserID), VisitID)"\
"SETTINGS index_granularity = 8192"

${CLIENT} -c $1 -q "CREATE DATABASE IF NOT EXISTS testdb ENGINE = Replicated('/clickhouse/databases/test1', 'shard1', 'replica3');"
${CLIENT} -c $1 -q "CREATE TABLE testdb.concurrent_test "\
"(CounterID UInt32, StartDate Date, UserID UInt32, VisitID UInt32, NestedColumn Nested(A UInt8, S String), ToDrop UInt32) "\
"ENGINE = MergeTree(StartDate, intHash32(UserID), (CounterID, StartDate, intHash32(UserID), VisitID), 8192);"
${CLIENT} -c $1 -q "CREATE TABLE testdb.dist AS testdb.concurrent_test ENGINE = Distributed(testdb, testdb, concurrent_test, CounterID)"
${CLIENT} -c $1 -q "ALTER TABLE testdb.concurrent_test ADD COLUMN Added0 UInt32;"
${CLIENT} -c $1 -q "ALTER TABLE testdb.concurrent_test ADD COLUMN Added2 UInt32;"
${CLIENT} -c $1 -q "ALTER TABLE testdb.concurrent_test ADD COLUMN Added1 UInt32 AFTER Added0;"
# ${CLIENT} -c $1 -q "ALTER TABLE testdb.concurrent_test ADD COLUMN AddedNested1 Nested(A UInt32, B UInt64) AFTER Added2;"
${CLIENT} -c $1 -q "ALTER TABLE testdb.concurrent_test ADD COLUMN AddedNested1.C Array(String) AFTER AddedNested1.B;"
${CLIENT} -c $1 -q "ALTER TABLE testdb.concurrent_test ADD COLUMN AddedNested2 Nested(A UInt32, B UInt64) AFTER AddedNested1;"

${CLIENT} -c $1 -q "CREATE TABLE testdb.concurrent_test(    CounterID UInt32,    StartDate Date,    UserID UInt32,"\
"    VisitID UInt32,    NestedColumn.A Array(UInt8),    NestedColumn.S Array(String),    ToDrop UInt32,"\
"    Added0 UInt32,    Added1 UInt32,    Added2 UInt32,    AddedNested1.A Array(UInt32),"\
"    AddedNested1.B Array(UInt64),    AddedNested1.C Array(String),    AddedNested2.A Array(UInt32),"\
"    AddedNested2.B Array(UInt64))"\
"ENGINE = MergeTree(StartDate, intHash32(UserID), (CounterID, StartDate, intHash32(UserID), VisitID), 8192)"

${CLIENT} -c $1 -q "DROP TABLE testdb.concurrent_test"
${CLIENT} -c $1 -q "CREATE TABLE testdb.concurrent_test "\
"(CounterID UInt32, StartDate Date, UserID UInt32, VisitID UInt32, NestedColumn Nested(A UInt8, S String), ToDrop UInt32) "\
"ENGINE = ReplicatedMergeTree ORDER BY CounterID;"

${CLIENT} -c $1 -q "CREATE TABLE testdb.concurrent_test(    CounterID UInt32,    StartDate Date,    UserID UInt32,"\
"    VisitID UInt32,    NestedColumn.A Array(UInt8),    NestedColumn.S Array(String),    ToDrop UInt32)"\
"ENGINE = ReplicatedMergeTree('/clickhouse/tables/uuid/{shard}', '{replica}')ORDER BY CounterIDSETTINGS index_granularity = 8192"
${CLIENT} -c $1 -q "INSERT INTO testdb.dist (CounterID, StartDate, UserID) SELECT number, addDays(toDate('2020-02-02'), number), intHash32(number) FROM numbers(10)"
${CLIENT} -c $1 -q "CREATE TABLE testdb.concurrent_test(    CounterID UInt32,    StartDate Date,    UserID UInt32,"\
"    VisitID UInt32,    NestedColumn.A Array(UInt8),    NestedColumn.S Array(String),    ToDrop UInt32)"\
"ENGINE = ReplicatedMergeTree('/clickhouse/tables/uuid/{shard}', '{replica}') ORDER BY CounterIDSETTINGS index_granularity = 8192"
${CLIENT} -c $1 -q "CREATE DATABASE testdb ENGINE = Replicated('/clickhouse/databases/test1', 'shard2', 'replica1');"
${CLIENT} -c $1 -q "CREATE DATABASE testdb ENGINE = Replicated('/clickhouse/databases/test1', 'shard2', 'replica2');"
${CLIENT} -c $1 -q "SYSTEM FLUSH DISTRIBUTED testdb.dist"
${CLIENT} -c $1 -q "ALTER TABLE testdb.concurrent_test UPDATE StartDate = addYears(StartDate, 1) WHERE 1"
${CLIENT} -c $1 -q "ALTER TABLE testdb.concurrent_test DELETE WHERE UserID % 2"
${CLIENT} -c $1 -q "SELECT shard_num, replica_num, host_name FROM system.clusters WHERE cluster='testdb'"

# test_drop_and_create_replica
${CLIENT} -c $1 -q "DROP DATABASE testdb SYNC"
${CLIENT} -c $1 -q "CREATE DATABASE testdb ENGINE = Replicated('/clickhouse/databases/test1', 'shard1', 'replica1');"
${CLIENT} -c $1 -q "CREATE TABLE testdb.concurrent_test(    CounterID UInt32,    StartDate Date,    UserID UInt32,"\
    "    VisitID UInt32,    NestedColumn.A Array(UInt8),    NestedColumn.S Array(String),    ToDrop UInt32)"\
    "ENGINE = ReplicatedMergeTree('/clickhouse/tables/uuid/{shard}', '{replica}')ORDER BY CounterIDSETTINGS index_granularity = 8192"
${CLIENT} -c $1 -q "SELECT CounterID, StartDate, UserID FROM testdb.dist ORDER BY CounterID"
${CLIENT} -c $1 -q "CREATE DATABASE recover ENGINE = Replicated('/clickhouse/databases/recover', 'shard1', 'replica1');"
${CLIENT} -c $1 -q "CREATE DATABASE recover ENGINE = Replicated('/clickhouse/databases/recover', 'shard1', 'replica2');"
${CLIENT} -c $1 -q "CREATE TABLE recover.t1 (n int) ENGINE=Memory"
${CLIENT} -c $1 -q "CREATE TABLE recover.t2 (s String) ENGINE=Memory"
${CLIENT} -c $1 -q "CREATE TABLE recover.mt1 (n int) ENGINE=MergeTree order by n"
${CLIENT} -c $1 -q "CREATE TABLE recover.mt2 (n int) ENGINE=MergeTree order by n"
${CLIENT} -c $1 -q "CREATE TABLE recover.rmt1 (n int) ENGINE=ReplicatedMergeTree order by n"
${CLIENT} -c $1 -q "CREATE TABLE recover.rmt2 (n int) ENGINE=ReplicatedMergeTree order by n"
${CLIENT} -c $1 -q "CREATE TABLE recover.rmt3 (n int) ENGINE=ReplicatedMergeTree order by n"
${CLIENT} -c $1 -q "CREATE TABLE recover.rmt5 (n int) ENGINE=ReplicatedMergeTree order by n"
${CLIENT} -c $1 -q "CREATE MATERIALIZED VIEW recover.mv1 (n int) ENGINE=ReplicatedMergeTree order by n AS SELECT n FROM recover.rmt1"
${CLIENT} -c $1 -q "CREATE MATERIALIZED VIEW recover.mv2 (n int) ENGINE=ReplicatedMergeTree order by n  AS SELECT n FROM recover.rmt2"
${CLIENT} -c $1 -q "CREATE DICTIONARY recover.d1 (n int DEFAULT 0, m int DEFAULT 1) PRIMARY KEY n "\
"SOURCE(CLICKHOUSE(HOST 'localhost' PORT 9000 USER 'default' TABLE 'rmt1' PASSWORD '' DB 'recover')) "\
"LIFETIME(MIN 1 MAX 10) LAYOUT(FLAT())"
${CLIENT} -c $1 -q "CREATE DICTIONARY recover.d2 (n int DEFAULT 0, m int DEFAULT 1) PRIMARY KEY n "\
"SOURCE(CLICKHOUSE(HOST 'localhost' PORT 9000 USER 'default' TABLE 'rmt2' PASSWORD '' DB 'recover')) "\
"LIFETIME(MIN 1 MAX 10) LAYOUT(FLAT())"

for table in 't1' 't2' 'mt1' 'mt2' 'rmt1' 'rmt2' 'rmt3' 'rmt5'; do
    ${CLIENT} -c $1 -q "INSERT INTO recover.$table VALUES (42)"
    ${CLIENT} -c $1 -q "INSERT INTO recover.$table VALUES (42)"
    ${CLIENT} -c $1 -q "SYSTEM SYNC REPLICA recover.table"
done

${CLIENT} -c $1 -q "RENAME TABLE recover.t1 TO recover.m1"
${CLIENT} -c $1 -q "RENAME TABLE recover.t1 TO recover.m1"
${CLIENT} -c $1 -q "ALTER TABLE recover.mt1  ADD COLUMN m int"
${CLIENT} -c $1 -q "ALTER TABLE recover.rmt1 ADD COLUMN m int"
${CLIENT} -c $1 -q "RENAME TABLE recover.rmt3 TO recover.rmt4"
${CLIENT} -c $1 -q "DROP TABLE recover.rmt5"
${CLIENT} -c $1 -q "DROP DICTIONARY recover.d2"
${CLIENT} -c $1 -q "CREATE DICTIONARY recover.d2 (n int DEFAULT 0, m int DEFAULT 1) PRIMARY KEY n "\
"SOURCE(CLICKHOUSE(HOST 'localhost' PORT 9000 USER 'default' TABLE 'rmt1' PASSWORD '' DB 'recover')) "\
"LIFETIME(MIN 1 MAX 10) LAYOUT(FLAT());"

inner_table=".inner_id." + dummy_node.query_with_retry("SELECT uuid FROM system.tables WHERE database='recover' AND name='mv1'").strip()
# ${CLIENT} -c $1 -q "ALTER TABLE recover.{} MODIFY COLUMN n int DEFAULT 42".format(inner_table), settings=settings)
# ${CLIENT} -c $1 -q "ALTER TABLE recover.mv1 MODIFY QUERY SELECT m FROM recover.rmt1".format(inner_table), settings=settings)
# ${CLIENT} -c $1 -q "RENAME TABLE recover.mv2 TO recover.mv3".format(inner_table), settings=settings)

${CLIENT} -c $1 -q "CREATE TABLE recover.tmp AS recover.m1"
${CLIENT} -c $1 -q "DROP TABLE recover.tmp"
${CLIENT} -c $1 -q "CREATE TABLE recover.tmp AS recover.m1"
${CLIENT} -c $1 -q "DROP TABLE recover.tmp"
${CLIENT} -c $1 -q "CREATE TABLE recover.tmp AS recover.m1"

${CLIENT} -c $1 -q "SELECT name FROM system.tables WHERE database='recover' AND name NOT LIKE '.inner_id.%' ORDER BY name"
${CLIENT} -c $1 -q "SELECT name, uuid, create_table_query FROM system.tables WHERE database='recover' AND name NOT LIKE '.inner_id.%' "\
"ORDER BY name SETTINGS show_table_uuid_in_table_create_query_if_not_nil=1"
${CLIENT} -c $1 -q "SELECT count() FROM system.tables WHERE database='recover' AND name LIKE '.inner_id.%'"
${CLIENT} -c $1 -q "SELECT count() FROM system.tables WHERE database='recover' AND name LIKE '.inner_id.%'"

for table in 'm1' 't2' 'mt1' 'mt2' 'rmt1' 'rmt2' 'rmt4' 'd1' 'd2' 'mv1' 'mv3'; do
    ${CLIENT} -c $1 -q "SELECT (*,).1 FROM recover.$table"
    ${CLIENT} -c $1 -q "SELECT (*,).1 FROM recover.$table"
    ${CLIENT} -c $1 -q "SELECT count() FROM recover.$table"
done

${CLIENT} -c $1 -q "SELECT count() FROM system.tables WHERE database='recover_broken_tables'"
${CLIENT} -c $1 -q "SHOW TABLES FROM recover_broken_tables LIKE 'mt1_29_%'"
${CLIENT} -c $1 -q "SELECT (*,).1 FROM recover_broken_tables.$table"
${CLIENT} -c $1 -q "SHOW TABLES FROM recover_broken_tables LIKE 'rmt5_29_%'"
${CLIENT} -c $1 -q "SELECT (*,).1 FROM recover_broken_tables.$table"

${CLIENT} -c $1 -q "DROP TABLE recover.tmp"
${CLIENT} -c $1 -q "SELECT count() FROM system.tables WHERE database='recover' AND name='tmp'"

${CLIENT} -c $1 -q "DROP DATABASE IF EXISTS testdb SYNC"
${CLIENT} -c $1 -q "DROP DATABASE IF EXISTS recover SYNC"
${CLIENT} -c $1 -q "CREATE DATABASE startup ENGINE = Replicated('/clickhouse/databases/startup', 'shard1', 'replica1');"
${CLIENT} -c $1 -q "CREATE DATABASE startup ENGINE = Replicated('/clickhouse/databases/startup', 'shard1', 'replica1');"
#main_node.query("CREATE TABLE startup.rmt (n int) ENGINE=ReplicatedMergeTree order by n")
${CLIENT} -c $1 -q "CREATE TABLE startup.rmt (n int) ENGINE=MergeTree order by n"
${CLIENT} -c $1 -q "INSERT INTO startup.rmt VALUES (42)"
${CLIENT} -c $1 -q "SELECT (*,).1 FROM startup.rmt"
${CLIENT} -c $1 -q "CREATE TABLE startup.m (n int) ENGINE=Memory"
${CLIENT} -c $1 -q "CREATE TABLE startup.m (n int) ENGINE=Memory"
${CLIENT} -c $1 -q "EXCHANGE TABLES startup.rmt AND startup.m"
${CLIENT} -c $1 -q "SELECT (*,).1 FROM startup.m"
