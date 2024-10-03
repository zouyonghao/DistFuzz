#! /bin/bash
set -x

for ((;;)) do
    /usr/lib/jvm/java-11-openjdk-amd64/bin/java -jar \
        /home/zyh/DistFuzz/rethinkdb_test/client/target/rethinkdb-test-client-1.0-SNAPSHOT-jar-with-dependencies.jar \
        5 set 0 && break
done
