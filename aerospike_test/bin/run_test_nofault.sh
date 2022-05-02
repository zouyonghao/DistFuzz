./start_event_control.sh
./env_init.sh
ulimit -n 20000
export NO_FAULT=1
/home/zyh/distributed-system-test/build/aerospike_test/aerospike_test_main init_random.txt 5 > log_test 2>&1
