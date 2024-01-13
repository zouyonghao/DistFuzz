mkdir -p test_cases/$1
mkdir -p test_cases/$1/run

pwd

rm ./strace_log*
cp log* ./test_cases/$1
cp *log ./test_cases/$1
mv random_node* ./test_cases/$1
mv random.txt ./test_cases/$1
mv init_random.txt ./test_cases/$1
rm -rf data* # data folder is too big to store.
mv rr_rec_* ./test_cases/$1

# checking the operation log!
{
  # ./check.sh ${PWD}/test_cases/$1/operation_log >${PWD}/test_cases/$1/check_log 2>&1

  if grep -q "slice bounds out of range" ./test_cases/$1/log_*; then
    echo "Find bug1"
    if [ -f found_bug1 ]; then
      echo "Bug1 has been found!"
    else
      echo "Bug1 has not been found!"
      touch found_bug1
      touch new_found
    fi
  fi
  if [ -f new_found ]; then
    echo "Find new bugs!"
    rm new_found
  else
    rm -rf ./test_cases/$1
  fi
}

# ./env_clear.sh
