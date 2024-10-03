./env_init.sh

../../build/fuzz/fuzzer -i input -o output -M fuzzer1 -f init_random.txt /bin/bash /root/DistFuzz/raftis_test/bin/run_test.sh

./env_clear.sh