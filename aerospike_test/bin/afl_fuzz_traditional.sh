./env_init.sh

../../build/fuzz/fuzzer -i input -o output -M fuzzer1 -f init_random.txt /bin/bash /home/zyh/distributed-system-test/aerospike_test/bin/run_test_nofault.sh

./env_clear.sh
