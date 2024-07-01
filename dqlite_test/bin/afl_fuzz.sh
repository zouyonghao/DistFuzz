export LIBRAFT_TRACE=1 LIBDQLITE_TRACE=1
NO_FAULT_FILES=yaml,probe
../../build/fuzz/fuzzer -b -i input -o output -M fuzzer1 -f init_random.txt \
    ../../build/dqlite_test/dqlite_test_main \
        --node_count=5 \
        --normal_count=4 --critic_count=3 \
        --check_after_fuzz=false