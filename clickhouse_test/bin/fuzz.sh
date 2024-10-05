# ./env_init.sh

export LC_ALL="C"
../../build/fuzz/fuzzer -b -i input -o output -M fuzzer1 -f init_random.txt \
    ../../build/clickhouse_test/clickhouse_test_main \
        --node_count=5 \
        --normal_count=3 --critic_count=2 \
        --check_after_fuzz=true

# ./env_clear.sh