bin_path=/home/zyh/NuRaft/build/examples

mkfifo calc1.pipe
mkfifo calc2.pipe
mkfifo calc3.pipe
sleep infinity >calc1.pipe &
sleep infinity >calc2.pipe &
sleep infinity >calc3.pipe &

# LD_PRELOAD=/home/zyh/distributed-system-test/build/preload_module/libdst_preload.so \
/home/zyh/strace/src/strace -f -o strace_log_0 bash -c "$bin_path/calc_server 1 127.0.1.1:10001 < calc1.pipe >> log0_0.log" 2>&1 &
# LD_PRELOAD=/home/zyh/distributed-system-test/build/preload_module/libdst_preload.so \
/home/zyh/strace/src/strace -f -o strace_log_1 bash -c "$bin_path/calc_server 2 127.0.1.1:10002 < calc2.pipe >> log1_0.log" 2>&1 &
# LD_PRELOAD=/home/zyh/distributed-system-test/build/preload_module/libdst_preload.so \
/home/zyh/strace/src/strace -f -o strace_log_2 bash -c "$bin_path/calc_server 3 127.0.1.1:10003 < calc3.pipe >> log2_0.log" 2>&1 &

sleep 1
echo add 2 127.0.1.1:10002 >calc1.pipe
sleep 1
echo add 3 127.0.1.1:10003 >calc1.pipe

# rm calc1.pipe
# rm calc2.pipe
# rm calc3.pipe
