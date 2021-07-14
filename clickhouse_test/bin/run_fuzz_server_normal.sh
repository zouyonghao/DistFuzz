bin_path=/home/zyh/ClickHouse/build/programs

mkdir -p run/zookeeper

./zkServer.sh start

$bin_path/clickhouse-server --config config0.xml &
$bin_path/clickhouse-server --config config1.xml &
$bin_path/clickhouse-server --config config2.xml &

./ensure_nodes.sh

/home/zyh/ClickHouse/build/programs/clickhouse-client -c config0.xml -q "CREATE DATABASE test"
/home/zyh/ClickHouse/build/programs/clickhouse-client -c config0.xml -q "CREATE TABLE test.test (key UInt32, v UInt32) ENGINE=ReplicatedMergeTree('/clickhouse/tables/{shard}/test', '{replica}') ORDER BY key"

/home/zyh/ClickHouse/build/programs/clickhouse-client -c config1.xml -q "CREATE DATABASE test"
/home/zyh/ClickHouse/build/programs/clickhouse-client -c config1.xml -q "CREATE TABLE test.test (key UInt32, v UInt32) ENGINE=ReplicatedMergeTree('/clickhouse/tables/{shard}/test', '{replica}') ORDER BY key"

/home/zyh/ClickHouse/build/programs/clickhouse-client -c config2.xml -q "CREATE DATABASE test"
/home/zyh/ClickHouse/build/programs/clickhouse-client -c config2.xml -q "CREATE TABLE test.test (key UInt32, v UInt32) ENGINE=ReplicatedMergeTree('/clickhouse/tables/{shard}/test', '{replica}') ORDER BY key"
