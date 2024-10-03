./start_event_control.sh
touch init_random.txt
/home/zyh/DistFuzz/build/braft_test/braft_test_main init_random.txt 5 > log_test 2>&1
./stop.sh