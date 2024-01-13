mkdir -p test_cases/$1
mkdir -p test_cases/$1/run

pwd

rm ./strace_log*
mv log* ./test_cases/$1
cp *log ./test_cases/$1
mv random_node* ./test_cases/$1
mv random.txt ./test_cases/$1
mv init_random.txt ./test_cases/$1
mv data* ./test_cases/$1
mv zoo* ./test_cases/$1
mv rr_rec_* ./test_cases/$1

# checking the operation log!
{
  # ./check.sh ${PWD}/test_cases/$1/operation_log >${PWD}/test_cases/$1/check_log 2>&1

  if grep -q "less than the current epoch" ./test_cases/$1/log_*; then
    echo "Find bug1"
  elif grep -q "unrecoverable" ./test_cases/$1/log_*; then
    echo "Find bug2"
  elif grep -q "NullPointerException" ./test_cases/$1/log_*; then
    echo "Find bug 3 NullPointerException!"
  elif grep -q "but next pending txn" ./test_cases/$1/log_*; then
    echo "Find bug 4"
  elif grep -q "ZooKeeperServer not running" ./test_cases/$1/log_*; then
    echo "Find bug 5"
  elif grep -q "Unable to read additional data from client" ./test_cases/$1/log_*; then
    echo "Find bug 6"
  else
    echo "no errors"
    rm -rf ./test_cases/$1
  fi
}

# ./env_clear.sh
