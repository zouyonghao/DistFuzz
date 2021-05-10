bin_path=/home/zyh/NuRaft/build/examples

mkfifo calc1.pipe
mkfifo calc2.pipe
mkfifo calc3.pipe
sleep infinity > calc1.pipe &
sleep infinity > calc2.pipe &
sleep infinity > calc3.pipe &

__DST_ENV_RANDOM_FILE__=random_node0.txt $bin_path/calc_server 1 localhost:10001 < calc1.pipe >> calc1.log 2>&1 &
__DST_ENV_RANDOM_FILE__=random_node1.txt $bin_path/calc_server 2 localhost:10002 < calc2.pipe >> calc2.log 2>&1 &
__DST_ENV_RANDOM_FILE__=random_node2.txt $bin_path/calc_server 3 localhost:10003 < calc3.pipe >> calc3.log 2>&1 &

sleep 1
echo add 2 localhost:10002 > calc1.pipe
sleep 1
echo add 3 localhost:10003 > calc1.pipe

# rm calc1.pipe
# rm calc2.pipe
# rm calc3.pipe