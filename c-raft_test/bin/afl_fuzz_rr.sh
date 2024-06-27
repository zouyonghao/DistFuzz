# ../../build/fuzz/fuzzer -i input -o output -M fuzzer1 -f init_random.txt \
#     /bin/bash /home/zyh/distributed-system-test/c-raft_test/bin/run_test.sh

../../build/fuzz/fuzzer -b -i input -o output -M fuzzer1 -f init_random.txt \
    ../../build/c-raft_test/c-raft_test_main \
        --node_count=5 \
        --normal_count=3 --critic_count=2 \
        --start_with_rr \
        --check_after_fuzz=true