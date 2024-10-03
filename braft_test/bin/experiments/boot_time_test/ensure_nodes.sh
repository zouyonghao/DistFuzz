#! /bin/bash
set -x

for ((;;)); do
{
    timeout 1 /home/zyh/braft/example/atomic/atomic_test \
        -conf="127.0.0.1:8300:0,127.0.0.1:8301:0,127.0.0.1:8302:0,127.0.0.1:8303:0,127.0.0.1:8304:0," \
        -atomic_op=get && break;
} &
timeout 1 /home/zyh/braft/example/atomic/atomic_test \
    -conf="127.0.0.1:8300:0,127.0.0.1:8301:0,127.0.0.1:8302:0,127.0.0.1:8303:0,127.0.0.1:8304:0," \
    -atomic_op=get && break;
done
