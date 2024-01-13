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
    if [ -f found_bug1 ]; then
      echo "Bug1 has been found!"
      rm -rf ./test_cases/$1
    else
      echo "Bug1 has not been found!"
      touch found_bug1
    fi
  elif grep -q "unrecoverable" ./test_cases/$1/log_*; then
    echo "Find bug2"
    if [ -f found_bug2 ]; then
      echo "Bug2 has been found!"
      rm -rf ./test_cases/$1
    else
      echo "Bug2 has not been found!"
      touch found_bug2
    fi
  elif grep -q "NullPointerException" ./test_cases/$1/log_*; then
    echo "Find bug 3 NullPointerException!"
    if [ -f found_bug3 ]; then
      echo "Bug3 has been found!"
      rm -rf ./test_cases/$1
    else
      echo "Bug3 has not been found!"
      touch found_bug3
    fi
  elif grep -q "but next pending txn" ./test_cases/$1/log_*; then
    echo "Find bug 4"
    if [ -f found_bug4 ]; then
      echo "Bug4 has been found!"
      rm -rf ./test_cases/$1
    else
      echo "Bug4 has not been found!"
      touch found_bug4
    fi
  elif grep -q "ZooKeeperServer not running" ./test_cases/$1/log_*; then
    echo "Find bug 5"
    if [ -f found_bug5 ]; then
      echo "Bug5 has been found!"
      rm -rf ./test_cases/$1
    else
      echo "Bug5 has not been found!"
      touch found_bug5
    fi
  elif grep -q "Unable to read additional data from client" ./test_cases/$1/log_*; then
    echo "Find bug 6"
    if [ -f found_bug6 ]; then
      echo "Bug6 has been found!"
      rm -rf ./test_cases/$1
    else
      echo "Bug6 has not been found!"
      touch found_bug6
    fi
  else
    echo "no errors"
    rm -rf ./test_cases/$1
  fi
}

# ./env_clear.sh
