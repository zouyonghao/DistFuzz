for (( ; ; )); do
    dd if=/dev/urandom of=init_random.txt bs=1K count=1
    ./start_event_control.sh
    /home/zyh/DistFuzz/build/etcd_test/etcd_test_main init_random.txt 5 > log_test 2>&1
    ./stop.sh
done
