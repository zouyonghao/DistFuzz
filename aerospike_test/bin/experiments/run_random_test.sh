for (( ; ; )); do
    ./generate_5k_bytes.sh init_random.txt
    /home/zyh/DistFuzz/build/aerospike_test/aerospike_test_main init_random.txt >log_test 2>&1
done
