./start_event_control.sh
export NO_FAULT=1
../../build/braft_test/braft_test_main --node_count=5 --normal_count=5 --critic_count=3 --check_after_fuzz=true > log_test 2>&1
./stop.sh