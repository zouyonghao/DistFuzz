#! /bin/bash
set -x

for ((;;)) do
    timeout 1 bash /home/zyh/distributed-system-test/redisraft_test/bin/run_client.sh set 0 && break
done
