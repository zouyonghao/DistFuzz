ulimit -s 81910
../../build/fuzz/fuzzer -i input -o output -M fuzzer1 -f init_random.txt \
    /bin/bash /home/zyh/distributed-system-test/braft_test/bin/run_test_nofault.sh
