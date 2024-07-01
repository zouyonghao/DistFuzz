mkdir -p test_cases/$1

pwd

# CHECK_RAFT_LOG_ERROR=1
# # check log
# if ./check_log.sh > log_check_raft 2>&1; then
#     CHECK_RAFT_LOG_ERROR=0
# fi

rm ./strace_log*
mv log* ./test_cases/$1
mv c-raft_data* ./test_cases/$1
mv random_node* ./test_cases/$1
mv random.txt ./test_cases/$1
mv init_random.txt ./test_cases/$1
# mv operation_log ./test_cases/$1
mv core* ./test_cases/$1
mv rr_rec* ./test_cases/$1

# checking the operation log!
{
  # ./check.sh ${PWD}/test_cases/$1/operation_log >${PWD}/test_cases/$1/check_log 2>&1
  # elif [ $CHECK_RAFT_LOG_ERROR == 1 ]; then
  #     echo "Check raft log failed!"
  if grep -q "ERROR: AddressSanitizer" ./test_cases/$1/log_app*; then
    echo "Find ASan errors!"
    if grep -q "0x81ea0" ./test_cases/$1/log_app*; then
      echo "Find a duplicated case"
      rm -rf ./test_cases/$1
    fi
  # elif grep -q '):' ./test_cases/$1/log_app*; then
  #   echo "Find errors!"
  # elif grep -q true ./test_cases/$1/check_log; then
  #     echo "No errors, deleting logs..."
  #     rm -rf ./test_cases/$1
  elif grep -q "Assert" ./test_cases/$1/log_*; then
    echo "Find assert failed!"
  elif grep -q "assert" ./test_cases/$1/log_*; then
    echo "Find assert failed!"
  elif grep -q "check failed!" ./test_cases/$1/log_test; then
    echo "Find check failed!"
  elif grep -q "all normal operators after fuzzing failed" ./test_cases/$1/log_test; then
    echo "Find all normal operators after fuzzing failed"
  else
    echo "No errors, deleting logs..."
    rm -rf ./test_cases/$1
  fi
  rm -rf ./test_cases/$1/c-raft_data*
} &
