for (( ; ; )); do
    dd if=/dev/urandom of=init_random.txt bs=5K count=1
    ./start_event_control.sh
    /home/zyh/DistFuzz/build/zookeeper_test/zookeeper_test_main init_random.txt > log_test 2>&1
    ./stop.sh
done
