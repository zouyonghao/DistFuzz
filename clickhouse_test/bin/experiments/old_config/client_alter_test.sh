set -x
CLIENT=/home/zyh/ClickHouse/build/programs/clickhouse-client

# ${CLIENT} -c $1 -q "CREATE DATABASE testdb ENGINE = Replicated('/clickhouse/databases/test1', 'shard1', 'replica1');"
# ${CLIENT} -c $1 -q "CREATE TABLE testdb.replicated_table (d Date, k UInt64, i32 Int32) ENGINE=ReplicatedMergeTree('/test/tmp', 'r') ORDER BY k PARTITION BY toYYYYMM(d);"
# ${CLIENT} -c $1 -q "CREATE TABLE testdb.replicated_table (d Date, k UInt64, i32 Int32) ENGINE=ReplicatedMergeTree('/test/tmp', 'r', d, k, 8192);"
# ${CLIENT} -c $1 -q "CREATE TABLE testdb.replicated_table (d Date, k UInt64, i32 Int32) ENGINE=ReplicatedMergeTree('/test/tmp/{shard}', '{replica}', d, k, 8192);"
# ${CLIENT} -c $1 -q "CREATE TABLE testdb.replicated_table (d Date, k UInt64, i32 Int32) ENGINE=ReplicatedMergeTree ORDER BY k PARTITION BY toYYYYMM(d);"
# ${CLIENT} -c $1 -q "CREATE TABLE testdb.replicated_table(d Date,k UInt64,i32 Int32)"\
# "ENGINE = ReplicatedMergeTree('/clickhouse/tables/uuid/{shard}', '{replica}')"\
# "PARTITION BY toYYYYMM(d) ORDER BY kSETTINGS index_granularity = 8192"

# ${CLIENT} -c $1 -q "show create testdb.replicated_table"

# engine=ReplicatedMergeTree
# name=testdb.alter_test_$engine

# ${CLIENT} -c $1 -q "CREATE TABLE $name "\
# "(CounterID UInt32, StartDate Date, UserID UInt32, VisitID UInt32, NestedColumn Nested(A UInt8, S String), ToDrop UInt32) "\
# "ENGINE = $engine PARTITION BY StartDate ORDER BY (CounterID, StartDate, intHash32(UserID), VisitID);"

${CLIENT} -c $1 -q "ALTER TABLE $2 ADD COLUMN $3 UInt32;"
# ${CLIENT} -c $1 -q "ALTER TABLE $name ADD COLUMN Added2 UInt32;"
# ${CLIENT} -c $1 -q "ALTER TABLE $name ADD COLUMN Added1 UInt32 AFTER Added0;"
${CLIENT} -c $1 -q "ALTER TABLE $$2 ADD COLUMN AddedNested1 Nested(A UInt32, B UInt64) AFTER $3;"
# ${CLIENT} -c $1 -q "ALTER TABLE $name ADD COLUMN AddedNested1.C Array(String) AFTER AddedNested1.B;"
# ${CLIENT} -c $1 -q "ALTER TABLE $name ADD COLUMN AddedNested2 Nested(A UInt32, B UInt64) AFTER AddedNested1;"

# ${CLIENT} -c $1 -q "CREATE DATABASE IF NOT EXISTS testdb ENGINE = Replicated('/clickhouse/databases/test1', 'shard1', 'replica3');"

# ${CLIENT} -c $1 -q "ALTER TABLE $name ADD COLUMN Added3 UInt32;"
# ${CLIENT} -c $1 -q "ALTER TABLE $name DROP COLUMN AddedNested1;"
# ${CLIENT} -c $1 -q "ALTER TABLE $name RENAME COLUMN Added1 TO AddedNested1;"
