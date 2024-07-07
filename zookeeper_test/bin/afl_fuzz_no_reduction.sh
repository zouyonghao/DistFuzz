../../build/fuzz/fuzzer -b -i input -o output -M fuzzer1 -f init_random.txt \
    ../../build/zookeeper_test/zookeeper_test_main \
    --node_count=5 \
    --normal_count=5 --critic_count=3 \
    --no_reduction \
    --check_after_fuzz=true
