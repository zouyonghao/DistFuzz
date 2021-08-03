bin_path=/home/zyh/braft/example/atomic

export DPDK_IP="10.0.1.2"
export DPDK_IP_GATEWAY="10.0.1.5"
# export DPDK_LO4_ENABLED=1
export DPDK_IP_MASK=24
export DPDK_VNIC="--vdev=virtio_user0,path=/dev/vhost-net,queue_size=1024,iface=veth1"
# export DPDK_PARAMS="--proc-type=secondary"
export DPDK_PARAMS="--file-prefix=node1"
LD_PRELOAD=/home/zyh/tldk/libtldk.so NODE_NAME=node1 "$bin_path"/atomic_server -reuse_addr -reuse_port -ip=10.0.1.2 -port=8300 -conf="10.0.1.1:8300:0,10.0.1.2:8300:0,10.0.1.1:8300:0," -data_path="./data1" > log1 2>&1 &

# export DPDK_IP="1.1.1.1"
# export DPDK_IP_GATEWAY="1.1.1.2"
# export DPDK_VNIC="--vdev=virtio_user0,path=/dev/vhost-net,queue_size=1024,iface=veth1"
# # export DPDK_PARAMS="--proc-type=secondary"
# export DPDK_PARAMS="--file-prefix=node1"
# LD_PRELOAD=/home/zyh/tldk/libtldk.so NODE_NAME=node1 "$bin_path"/atomic_server -reuse_addr -reuse_port -ip=1.1.1.1 -port=8301 -conf="1.1.1.1:8300:0,1.1.1.1:8301:0,1.1.1.1:8302:0," -data_path="./data1" > log1 2>&1 &
