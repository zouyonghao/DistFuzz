mkdir -p test_cases/$1
mkdir -p test_cases/$1/run

pwd

mv log* ./test_cases/$1
mv random_node* ./test_cases/$1
mv random.txt ./test_cases/$1
mv init_random.txt ./test_cases/$1
mv strace* ./test_cases/$1

if grep -q "check failed!" ./test_cases/$1/log_test; then
  echo "Find check failed!"
else
  echo "check success."
  rm -rf ./test_cases/$1
fi
