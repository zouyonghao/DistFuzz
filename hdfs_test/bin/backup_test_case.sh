mkdir -p test_cases/$1
mkdir -p test_cases/$1/run

pwd

rm ./a
rm ./strace_log*
# cp *log ./test_cases/$1
mv random_node* ./test_cases/$1
mv random.txt ./test_cases/$1
mv init_random.txt ./test_cases/$1
# mv data* ./test_cases/$1
# mv zoo* ./test_cases/$1
# mv rr_rec_* ./test_cases/$1
mv hadoop ./test_cases/$1
mv core* ./test_cases/$1

# checking the operation log!
./check.sh ${PWD}/test_cases/$1/operation_log >${PWD}/test_cases/$1/check_log 2>&1

if grep -q "check failed!" ./test_cases/$1/log_test; then
  echo "Find check failed!"
elif grep -a 'ERROR' ./test_cases/$1/hadoop/hadoop*; then
  echo "Find ERROR!"
elif grep -q "NullPointerException" ./test_cases/$1//hadoop/hadoop*; then
  echo "Find NullPointerException!"
elif grep -q true ./test_cases/$1/check_log; then
  echo "No errors, deleting logs..."
  rm -rf ./test_cases/$1
else
  echo "Find operation check fail."
  rm -rf ./test_cases/$1
fi

mv log* ./test_cases/$1

# ./env_clear.sh
