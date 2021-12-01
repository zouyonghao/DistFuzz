for (( ; ; )); do
    dd if=/dev/urandom of=init_random.txt bs=1M count=1
    ./start_event_control.sh
    /home/zyh/distributed-system-test/build/etcd_test-3517/etcd_test_main-3517 init_random.txt 5 > log_test 2>&1
    ./stop.sh
done
