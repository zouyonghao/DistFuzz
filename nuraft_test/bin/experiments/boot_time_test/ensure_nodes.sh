#! /bin/bash
set -x

export PIPE_FOLDER=data/
export LOG_FOLDER=data/

for ((;;)) do
    timeout 2 bash /home/zyh/DistFuzz/nuraft_test/bin/run_client.sh get && break
done
