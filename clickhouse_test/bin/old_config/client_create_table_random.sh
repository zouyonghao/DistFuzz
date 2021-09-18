/home/zyh/ClickHouse/build/programs/clickhouse-client -c $1 -q "CREATE TABLE $8.$2 ($3 Date, $4 UInt64, $5 Int32)"\
"ENGINE = ReplicatedMergeTree('/clickhouse/tables/uuid/$6', '$7')"\
"PARTITION BY toYYYYMM($2) ORDER BY kSETTINGS index_granularity = 8192"