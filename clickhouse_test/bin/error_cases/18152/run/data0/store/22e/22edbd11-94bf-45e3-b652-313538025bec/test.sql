ATTACH TABLE _ UUID 'd54ea559-79b3-4e66-9738-d02720213384'
(
    `key` UInt32,
    `v` UInt32
)
ENGINE = ReplicatedMergeTree('/clickhouse/tables/{shard}/test', '{replica}')
ORDER BY key
SETTINGS index_granularity = 8192
