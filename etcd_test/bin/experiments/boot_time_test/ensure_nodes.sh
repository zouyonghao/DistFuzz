#! /bin/bash
set -x

for ((;;)); do
    /home/zyh/etcd/etcdctl \
        --endpoints=127.0.1.1:2380,127.0.1.1:2381,127.0.1.1:2382,127.0.1.1:2383,127.0.1.1:2384 \
        put a 123 && break;
done
