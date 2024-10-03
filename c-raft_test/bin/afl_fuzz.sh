# ../../build/fuzz/fuzzer -i input -o output -M fuzzer1 -f init_random.txt \
#     /bin/bash /home/zyh/DistFuzz/c-raft_test/bin/run_test.sh
export LIBRAFT_TRACE=1 LIBDQLITE_TRACE=1
../../build/fuzz/fuzzer -b -i input -o output -M fuzzer1 -f init_random.txt \
    ../../build/c-raft_test/c-raft_test_main \
        --node_count=5 \
        --normal_count=3 --critic_count=2 \
        --check_after_fuzz=true