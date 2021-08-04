bin_path=/home/zyh/braft/example/atomic

export DPDK_IP="10.0.1.4"
export DPDK_IP_GATEWAY="10.0.1.1"
# export DPDK_LO4_ENABLED=1
export DPDK_IP_MASK=24
export DPDK_VNIC="--vdev=virtio_user0,path=/dev/vhost-net,queue_size=1024,iface=veth0"
# export DPDK_PARAMS="--proc-type=primary"
export DPDK_PARAMS="--file-prefix=node0"
LD_PRELOAD=/home/zyh/tldk/libtldk.so NODE_NAME=node0 "$bin_path"/atomic_server -reuse_addr -reuse_port -ip=10.0.1.4 -port=8300 -conf="10.0.1.4:8300:0,10.0.1.5:8300:0,10.0.1.6:8300:0," -data_path="./data0" > log0 2>&1 &
