ulimit -s 81910
export NO_FAULT_FILES=tmp
../../build/fuzz/fuzzer -b -i input -o output -M fuzzer1 -f init_random.txt \
    ../../build/braft_test/braft_test_main \
        --node_count=3 \
        --normal_count=2 --critic_count=3 \
        --check_after_fuzz=true \
        --start_with_rr