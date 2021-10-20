mkdir -p test_cases/$1

pwd

mv run ./test_cases/$1
mv log* ./test_cases/$1
mv *log ./test_cases/$1
mv random_node* ./test_cases/$1
mv random.txt ./test_cases/$1
mv init_random.txt ./test_cases/$1
mv operation_log ./test_cases/$1

# checking the operation log!
{
  # ./check.sh ${PWD}/test_cases/$1/operation_log >${PWD}/test_cases/$1/check_log 2>&1
  if grep -q 'ERROR' ./test_cases/$1/run/log[0-9]; then
    echo "Find errors!"
  elif grep -q 'BUG' ./test_cases/$1/run/log*; then
    echo "Find BUGS!"
  elif grep 'CRITIC' ./test_cases/$1/run/log* | grep -v fabric; then
    echo "Find CRITIC!"
  elif grep -q "ERROR: AddressSanitizer" ./test_cases/$1/run/log*; then
    echo "Find ASan errors!"
  elif grep -q "AddressSanitizer" ./test_cases/$1/run/log*; then
    echo "Find ASan errors!"
  elif grep -q "AddressSanitizer" ./test_cases/$1/log*; then
    echo "Find ASan errors!"
  elif grep -q "check failed!" ./test_cases/$1/log_test; then
    echo "Find check failed!"
  elif grep -q "all normal operators after fuzzing failed" ./test_cases/$1/log_test; then
    echo "Find all normal operators after fuzzing failed"
  # elif grep -q false ./test_cases/$1/check_log; then
  #     echo "find operation log errors, bug ignore for now!"
  #     # rm -rf ./test_cases/$1
  else
    echo "No errors found."
    rm -rf ./test_cases/$1
  fi
} &
