#! /bin/bash
set -x
for (( ; ; )) do /home/zyh/ClickHouse/build/programs/clickhouse-client -c config0.xml -q "select 1" && break; done
for (( ; ; )) do /home/zyh/ClickHouse/build/programs/clickhouse-client -c config1.xml -q "select 1" && break; done
for (( ; ; )) do /home/zyh/ClickHouse/build/programs/clickhouse-client -c config2_for_proxy.xml -q "select 1" && break; done
