export NO_FAULT_FILES=xml,html,jar
../../build/fuzz/fuzzer -b -i input -o output -M fuzzer1 -f init_random.txt \
    ../../build/nuraft_test/nuraft_test_main \
    --node_count=5 \
    --normal_count=3 --critic_count=2 \
    --check_after_fuzz=false
