echo "Test local network delay"
../build/test/test_network_delay_server 127.0.0.1 8300 &
../build/test/test_network_delay_client 127.0.0.1 8300 0.0.0.0

sleep 1

# echo "Test user-level network delay (out network)"

# DPDK_IP="10.0.1.4" \
#     DPDK_IP_GATEWAY="10.0.1.1" \
#     DPDK_IP_MASK=24 \
#     DPDK_VNIC="--vdev=virtio_user0,path=/dev/vhost-net,queue_size=1024,iface=veth0" \
#     DPDK_PARAMS="--file-prefix=node0" \
#     LD_PRELOAD=/home/zyh/tldk/libtldk.so ../build/test/test_network_delay_server 10.0.1.4 8300 &

# sleep 5
# ../build/test/test_network_delay_client 10.0.1.4 8300 0.0.0.0

sleep 5
echo "Test user-level network delay (in network?)"

DPDK_IP="10.0.1.4" \
    DPDK_IP_GATEWAY="10.0.1.1" \
    DPDK_IP_MASK=24 \
    DPDK_VNIC="--vdev=virtio_user0,path=/dev/vhost-net,queue_size=1024,iface=veth0" \
    DPDK_PARAMS="--file-prefix=node0" \
    LD_PRELOAD=/home/zyh/tldk/libtldk.so ../build/test/test_network_delay_server 10.0.1.4 8300 &

sleep 10
DPDK_IP="10.0.1.5" \
    DPDK_IP_GATEWAY="10.0.1.2" \
    DPDK_IP_MASK=24 \
    DPDK_VNIC="--vdev=virtio_user0,path=/dev/vhost-net,queue_size=1024,iface=veth1" \
    DPDK_PARAMS="--file-prefix=node1" \
    LD_PRELOAD=/home/zyh/tldk/libtldk.so ../build/test/test_network_delay_client 10.0.1.4 8300 0.0.0.0


# sleep 1
# echo "Test proxy network delay"
# ../build/test/test_network_delay_server 127.0.0.1 8300 &
# ../build/test/test_network_delay_client 127.0.0.1 8300
