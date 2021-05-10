bin_path=/home/zyh/NuRaft/build/examples

__DST_ENV_RANDOM_FILE__=random_node0.txt $bin_path/calc_server 1 localhost:10001 < calc1.pipe >> calc1.log 2>&1 &