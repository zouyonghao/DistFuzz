#! /bin/bash
set -x

for ((;;)) do
    tail -n 2 data/log0 | grep "count 30" && break
    tail -n 2 data/log1 | grep "count 30" && break
    tail -n 2 data/log2 | grep "count 30" && break
    tail -n 2 data/log3 | grep "count 30" && break
    tail -n 2 data/log4 | grep "count 30" && break
done
