../../build/fuzz/fuzzer -b -i input -o output -M fuzzer1 -f init_random.txt \
    ../../build/etcd_test/etcd_test_main \
    --node_count=3 \
    --normal_count=2 --critic_count=2 \
    --start_with_rr
