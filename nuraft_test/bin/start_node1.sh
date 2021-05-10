bin_path=/home/zyh/NuRaft/build/examples

__DST_ENV_RANDOM_FILE__=random_node1.txt $bin_path/calc_server 2 localhost:10002 < calc2.pipe >> calc2.log 2>&1 &