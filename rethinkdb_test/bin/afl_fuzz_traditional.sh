export NO_FAULT_FILES=tun
../../build/fuzz/fuzzer -i input -o output -M fuzzer1 -f init_random.txt \
    ../../build/rethinkdb_test/rethinkdb_test_main \
        --node_count=5 \
        --normal_count=5 --critic_count=3 \
        --check_after_fuzz=true