mkdir -p test_cases/$1

pwd

mv log* ./test_cases/$1
mv rethinkdb_data* ./test_cases/$1
mv random_node* ./test_cases/$1
mv random.txt ./test_cases/$1
mv operation_log ./test_cases/$1

# checking the operation log!
{
  ./check.sh ${PWD}/test_cases/$1/operation_log >${PWD}/test_cases/$1/check_log 2>&1
  if grep -q ^E ./test_cases/$1/log_app*; then
    echo "Find errors!"
  elif grep 'error: Error in' ./test_cases/$1/log_app* | grep -v "cluster.cc at line 1348"; then
    echo "May find error trace and it's not error 6!"
  elif grep -q "ERROR: AddressSanitizer" ./test_cases/$1/log_app*; then
    echo "Find ASan errors!"
#  elif grep -q "check failed!" ./test_cases/$1/log_test; then
#    echo "Find check failed!"
#  elif grep -q "all normal operators after fuzzing failed" ./test_cases/$1/log_test; then
#    echo "Find all normal operators after fuzzing failed"
  elif grep -q false ./test_cases/$1/check_log; then
    echo "Find operation log errors!"
  else
    echo "Seems no errors, deleting logs..."
    rm -rf ./test_cases/$1
  fi
} &
