mkdir -p test_cases/$1
mkdir -p test_cases/$1/run

pwd

mv log* ./test_cases/$1
cp *log ./test_cases/$1
mv random_node* ./test_cases/$1
mv random.txt ./test_cases/$1
mv init_random.txt ./test_cases/$1
mv data* ./test_cases/$1

# checking the operation log!
{
  ./check.sh ${PWD}/test_cases/$1/operation_log >${PWD}/test_cases/$1/check_log 2>&1

  if grep -a 'ERROR' ./test_cases/$1/log_app*; then
    echo "Find ERROR!"
  elif grep -q "check failed!" ./test_cases/$1/log_test; then
    echo "Find check failed!"
  elif grep -q "all normal operators after fuzzing failed" ./test_cases/$1/log_test; then
    echo "Find all normal operators after fuzzing failed"
  elif grep Bad ./test_cases/$1/check_before_stop.log; then
    # For ZOOKEEPER-2212
    echo "Find bad leaders or observers!"
  elif grep -q true ./test_cases/$1/check_log; then
    echo "No errors, deleting logs..."
    rm -rf ./test_cases/$1
  else
    echo "Find operation check fail."
    # rm -rf ./test_cases/$1
  fi
} &

# ./env_clear.sh
