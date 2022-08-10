general_test_main \
  --node_count=1 \
  --normal_count=5 \
  --normal_sleep_ms=10000000 \
  --critic_count=0 \
  --check_after_fuzz=false \
  --fuzz_before_init=true \
  >log_test 2>&1
