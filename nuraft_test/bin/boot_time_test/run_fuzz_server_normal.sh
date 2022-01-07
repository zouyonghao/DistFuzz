bin_path=/home/zyh/NuRaft/build/examples

mkdir data

mkfifo data/calc1.pipe
mkfifo data/calc2.pipe
mkfifo data/calc3.pipe
sleep infinity >data/calc1.pipe &
sleep infinity >data/calc2.pipe &
sleep infinity >data/calc3.pipe &

setsid $bin_path/calc_server 1 127.0.1.1:10001 < data/calc1.pipe >> data/log0_0.log 2>&1 &
setsid $bin_path/calc_server 2 127.0.1.1:10002 < data/calc2.pipe >> data/log1_0.log 2>&1 &
setsid $bin_path/calc_server 3 127.0.1.1:10003 < data/calc3.pipe >> data/log2_0.log 2>&1 &

sleep 1
echo add 2 127.0.1.1:10002 >data/calc1.pipe
sleep 1
echo add 3 127.0.1.1:10003 >data/calc1.pipe

# rm calc1.pipe
# rm calc2.pipe
# rm calc3.pipe
