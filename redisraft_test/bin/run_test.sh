./env_init.sh
/home/zyh/DistFuzz/build/redisraft_test/redisraft_test_main init_random.txt 5 > log_test 2>&1
./env_clear.sh
sleep 1