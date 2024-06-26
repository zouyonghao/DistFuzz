# ../../build/fuzz/fuzzer -i input -o output -M fuzzer1 -f init_random.txt \
#     /bin/bash /home/zyh/distributed-system-test/c-raft_test/bin/run_test.sh

../../build/fuzz/fuzzer -b -i input -o output -M fuzzer1 -f init_random.txt \
    ../../build/braft_test/braft_test_main \
        --node_count=5 \
        --normal_count=5 --critic_count=3 \
        --check_after_fuzz=true