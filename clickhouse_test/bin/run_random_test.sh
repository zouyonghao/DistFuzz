for (( ; ; )); do
    ./generate_5k_bytes.sh init_random.txt
    /home/zyh/distributed-system-test/build/clickhouse_test/clickhouse_test_main init_random.txt >log_test 2>&1
done