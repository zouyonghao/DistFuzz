mkdir -p test_cases/$1

pwd

mv log* ./test_cases/$1
mv data* ./test_cases/$1
mv random_node* ./test_cases/$1
mv random.txt ./test_cases/$1
mv init_random.txt ./test_cases/$1
mv operation_log ./test_cases/$1
mv strace_log* ./test_cases/$1

# checking the operation log!
{
  # ./check.sh ${PWD}/test_cases/$1/operation_log >${PWD}/test_cases/$1/check_log 2>&1

  if grep -q "detected memory leaks" ./test_cases/$1/log_*; then
    echo "Find bug1"
    if [ -f found_bug1 ]; then
      echo "Bug1 has been found!"
    else
      echo "Bug1 has not been found!"
      touch found_bug1
      touch new_found
    fi
  fi
  if grep -q "Fail to rename" ./test_cases/$1/log_*; then
    echo "Find bug2"
    if [ -f found_bug2 ]; then
      echo "Bug2 has been found!"
    else
      echo "Bug2 has not been found!"
      touch found_bug2
      touch new_found
    fi
  fi
  if grep -q "Met raft error" ./test_cases/$1/log_*; then
    echo "Find bug 3!"
    if [ -f found_bug3 ]; then
      echo "Bug3 has been found!"
    else
      echo "Bug3 has not been found!"
      touch found_bug3
      touch new_found
    fi
  fi
  if grep -q "meta.term == header.term" ./test_cases/$1/log_*; then
    echo "Find bug 4"
    if [ -f found_bug4 ]; then
      echo "Bug4 has been found!"
    else
      echo "Bug4 has not been found!"
      touch found_bug4
      touch new_found
    fi
  fi
  if grep -q "Check failed: false" ./test_cases/$1/log_*; then
    echo "Find bug 5"
    if [ -f found_bug5 ]; then
      echo "Bug5 has been found!"
    else
      echo "Bug5 has not been found!"
      touch found_bug5
      touch new_found
    fi
  fi
  if grep -q "reject term_unmatched AppendEntries" ./test_cases/$1/log_*; then
    echo "Find bug 6"
    if [ -f found_bug6 ]; then
      echo "Bug6 has been found!"
    else
      echo "Bug6 has not been found!"
      touch found_bug6
      touch new_found
    fi
  fi
  if grep -q "AddressSanitizer: SEGV" ./test_cases/$1/log_*; then
    echo "Find bug 8"
    if [ -f found_bug8 ]; then
      echo "Bug8 has been found!"
    else
      echo "Bug8 has not been found!"
      touch found_bug8
      touch new_found
    fi
  fi
  if grep -q "Check failed: 0" ./test_cases/$1/log_*; then
    echo "Find bug 9"
    if [ -f found_bug6 ]; then
      echo "Bug6 has been found!"
    else
      echo "Bug6 has not been found!"
      touch found_bug6
      touch new_found
    fi
  fi
  if grep -q "heap-use-after-free" ./test_cases/$1/log_*; then
    echo "Find bug 9"
    if [ -f found_bug6 ]; then
      echo "Bug6 has been found!"
    else
      echo "Bug6 has not been found!"
      touch found_bug6
      touch new_found
    fi
  fi
  if [ -f new_found ]; then
    echo "new found"
    rm new_found
  else
    rm -rf ./test_cases/$1
  fi
}

