./start_event_control.sh
export NO_FAULT_FILES=tmp
../../build/braft_test/braft_test_main --normal_sleep_ms=200 --no_fuzz_time --critic_sleep_ms=2000 --node_count=5 --normal_count=5 --critic_count=3 --check_after_fuzz=true > log_test 2>&1
./stop.sh
