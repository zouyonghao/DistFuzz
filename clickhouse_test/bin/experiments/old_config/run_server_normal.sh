bin_path=/home/zyh/ClickHouse/build/programs

mkdir -p run/zookeeper

./zkServer.sh start

$bin_path/clickhouse-server --config config0.xml &
$bin_path/clickhouse-server --config config1.xml &
$bin_path/clickhouse-server --config config2.xml &

./env_init.sh