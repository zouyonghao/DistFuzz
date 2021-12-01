for ((i = 0; i < 100; i++)); do
  echo "run $i"
  /home/zyh/distributed-system-test/build/etcd_test-3517/etcd_test_main-3517 error_cases/267/init_random.txt 5 > ./log_test 2>&1
  if grep 'client operator got timeout' ./log_test; then
    echo "get timeout!" >>test
  fi
  ./stop.sh
done
