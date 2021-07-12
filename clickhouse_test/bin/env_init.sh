
for (( ; ; )) do /home/zyh/ClickHouse/build/programs/clickhouse-client -c config0.xml -q "select 1" && break; done
for (( ; ; )) do /home/zyh/ClickHouse/build/programs/clickhouse-client -c config1.xml -q "select 1" && break; done
for (( ; ; )) do /home/zyh/ClickHouse/build/programs/clickhouse-client -c config2_for_proxy.xml -q "select 1" && break; done

/home/zyh/ClickHouse/build/programs/clickhouse-client -c config0.xml -q "CREATE DATABASE test"
/home/zyh/ClickHouse/build/programs/clickhouse-client -c config0.xml -q "CREATE TABLE test.test (key UInt32, v UInt32) ENGINE=ReplicatedMergeTree('/clickhouse/tables/{shard}/test', '{replica}') ORDER BY key"

/home/zyh/ClickHouse/build/programs/clickhouse-client -c config1.xml -q "CREATE DATABASE test"
/home/zyh/ClickHouse/build/programs/clickhouse-client -c config1.xml -q "CREATE TABLE test.test (key UInt32, v UInt32) ENGINE=ReplicatedMergeTree('/clickhouse/tables/{shard}/test', '{replica}') ORDER BY key"

/home/zyh/ClickHouse/build/programs/clickhouse-client -c config2.xml -q "CREATE DATABASE test"
/home/zyh/ClickHouse/build/programs/clickhouse-client -c config2.xml -q "CREATE TABLE test.test (key UInt32, v UInt32) ENGINE=ReplicatedMergeTree('/clickhouse/tables/{shard}/test', '{replica}') ORDER BY key"
