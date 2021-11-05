for ((i = 0; i < 100; i++)); do
  /home/zyh/distributed-system-test/build/zookeeper_test/zookeeper_test_main test_cases/11746/init_random.txt 3
  if grep 'Bad observer' ./check_before_stop.log; then
    echo "bad!" >>test
  fi
done
