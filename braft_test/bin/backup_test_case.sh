mkdir -p test_cases/$1

pwd

# CHECK_RAFT_LOG_ERROR=1
# # check log
# if ./check_log.sh > log_check_raft 2>&1; then
#     CHECK_RAFT_LOG_ERROR=0
# fi

mv log* ./test_cases/$1
mv data* ./test_cases/$1
mv random_node* ./test_cases/$1
mv random.txt ./test_cases/$1
mv init_random.txt ./test_cases/$1
mv operation_log ./test_cases/$1
mv strace_log* ./test_cases/$1

# checking the operation log!
{
  timeout 10 ./check.sh ${PWD}/test_cases/$1/operation_log >${PWD}/test_cases/$1/check_log 2>&1
  
  if grep -q "check failed!" ./test_cases/$1/log_test; then
    echo "Find check failed!"
  elif grep -q "all normal operators after fuzzing failed" ./test_cases/$1/log_test; then
    echo "Find all normal operators after fuzzing failed"
  # elif [[ $CHECK_RAFT_LOG_ERROR == 1 ]]; then
  #     echo "Check raft log failed!"
  elif grep -q "Sanitizer" ./test_cases/$1/log*; then
    echo "Find ASan errors!"
  elif grep -q ^F ./test_cases/$1/log_app_*; then
    echo "Find fatal!"
  #   elif grep -q "DANGEROUS" ./test_cases/$1/log*; then
  #     echo "Find DANGEROUS!"
  elif grep -q false ./test_cases/$1/check_log; then
    echo "Find operation log errors!"
  else
    # echo "Find operation log errors!"
    echo "No errors, deleting logs..."
    rm -rf ./test_cases/$1
  fi
  
  # rm -rf ./test_cases/$1
} 
