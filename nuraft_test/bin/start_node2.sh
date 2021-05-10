bin_path=/home/zyh/NuRaft/build/examples

__DST_ENV_RANDOM_FILE__=random_node2.txt $bin_path/calc_server 3 localhost:10004 < calc3.pipe >> calc3.log 2>&1 &