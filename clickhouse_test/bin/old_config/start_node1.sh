bin_path=/home/zyh/ClickHouse/build/programs

export CLICKHOUSE_WATCHDOG_ENABLE=0
__DST_ENV_RANDOM_FILE__=random_node1.txt setsid $bin_path/clickhouse-server --config config1.xml < /dev/null &> run/t_log1 &
disown
