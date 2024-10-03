bin_path=/home/zyh/braft/example/atomic

LD_PRELOAD=/home/zyh/DistFuzz/build/preload_module/libdst_preload.so \
__DST_ENV_RANDOM_FILE__=random_node1.txt \
NODE_NAME=node1 \
NODE_ID=1 \
"$bin_path"/atomic_server -reuse_addr -ip=127.0.1.1 -reuse_port -port=8301 -election_timeout_ms=200 -conf="127.0.1.1:8300:0,127.0.1.1:8301:0,127.0.1.1:8302:0," -data_path="./data1" >> log1 2>&1 &
