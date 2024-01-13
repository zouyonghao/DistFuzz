echo "start: test_case_count"
cat test_case_count
for ((i = 0; i < 100; i++)); do
  /home/zyh/distributed-system-test/build/etcd_test/etcd_test_main \
    --node_count=5 \
    --normal_count=5 --critic_count=3 \
    --check_after_fuzz=true \
    --random_file=./test_cases/$1/init_random.txt
done
echo "end: test_case_count"
cat test_case_count

