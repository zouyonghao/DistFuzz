#! /bin/bash
set -x

for ((;;)); do
    /usr/share/zookeeper/bin/zkCli.sh -server \
        127.0.1.1:2181,127.0.1.1:2182,127.0.1.1:2183,127.0.1.1:2184,127.0.1.1:2185 \
        set /a 1 2>/dev/null && break;
done
