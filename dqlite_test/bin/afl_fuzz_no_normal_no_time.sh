export LIBRAFT_TRACE=1 LIBDQLITE_TRACE=1
export NO_FAULT_FILES=yaml,probe,join,dqlite_data
../../build/fuzz/fuzzer -b -i input -o output -M fuzzer1 -f init_random.txt \
    ../../build/dqlite_test/dqlite_test_main \
        --node_count=5 \
        --normal_count=4 --critic_count=5 \
        --no_fuzz_normal_events --no_fuzz_time \
        --check_after_fuzz=false