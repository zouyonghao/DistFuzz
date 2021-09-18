/home/zyh/ClickHouse/build/programs/clickhouse-client -c $1 -q "drop database $2"

timeout 2 /home/zyh/ClickHouse/build/programs/clickhouse-client -c $1 -q "drop TABLE testdb.$2"
timeout 2 /home/zyh/ClickHouse/build/programs/clickhouse-client -c $1 -q "drop TABLE test.$2"