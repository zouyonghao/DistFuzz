ulimit -s 81910
export NO_FAULT_FILES=tmp
../../build/fuzz/fuzzer -i input -o output -M fuzzer1 -f init_random.txt \
    ../../build/braft_test/braft_test_main \
        --node_count=5 \
        --normal_count=3 --critic_count=5 \
        --start_with_rr
