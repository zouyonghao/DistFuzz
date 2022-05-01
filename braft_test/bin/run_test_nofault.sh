./start_event_control.sh
export NO_FAULT=1
/home/zyh/distributed-system-test/build/braft_test/braft_test_main init_random.txt 5 > log_test 2>&1
./stop.sh