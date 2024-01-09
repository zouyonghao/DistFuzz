ulimit -n 20000

sudo tunctl -t tap0
sudo ifconfig tap0 127.0.1.1/24 up

../../build/fuzz/fuzzer -b -i input -o output -M fuzzer1 -f init_random.txt \
    ../../build/aerospike_test/aerospike_test_main \
        --node_count=5 \
        --normal_count=5 --critic_count=3 \
        --check_after_fuzz=true