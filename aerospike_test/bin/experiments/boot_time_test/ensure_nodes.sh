#! /bin/bash
set -x

for ((;;)); do
    ../../../build/aerospike_test/example_client && break;
done
