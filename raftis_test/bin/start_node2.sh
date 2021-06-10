bin_path=/root/floyd/floyd/example/redis/output/bin

__DST_ENV_RANDOM_FILE__=random_node0.txt $bin_path/raftis \
    "127.0.0.1:5005,127.0.0.1:5006,127.0.0.1:5004" \
    "127.0.0.1" 5004 "./data3/" 6579 &