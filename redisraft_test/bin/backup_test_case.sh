mkdir -p test_cases/$1

pwd

mv log* ./test_cases/$1
mv *.db ./test_cases/$1
mv *.db.idx ./test_cases/$1
mv random_node* ./test_cases/$1
mv random.txt ./test_cases/$1
mv init_random.txt ./test_cases/$1
mv operation_log ./test_cases/$1

# checking the operation log!
{
  ./check.sh ${PWD}/test_cases/$1/operation_log >${PWD}/test_cases/$1/check_log 2>&1
  if grep -q 'ERROR' ./test_cases/$1/log*; then
    echo "Find errors!"
  elif grep -q "c->flags & REDIS_SUBSCRIBED || c->flags & REDIS_MONITORING" ./test_cases/$1/log*; then
    echo "Find duplicated cases"
    rm -rf ./test_cases/$1
  elif grep -q 'BUG' ./test_cases/$1/log*; then
    echo "Find BUGS!"
  elif grep -q "ERROR: AddressSanitizer" ./test_cases/$1/log*; then
    echo "Find ASan errors!"
  elif grep -q "check failed!" ./test_cases/$1/log_test; then
    echo "Find check failed!"
  elif grep -q "all normal operators after fuzzing failed" ./test_cases/$1/log_test; then
    echo "Find all normal operators after fuzzing failed"
  elif grep -q true ./test_cases/$1/check_log; then
    echo "No errors, deleting logs..."
    rm -rf ./test_cases/$1
  else
    echo "Find operation log errors!"
  fi
} &
