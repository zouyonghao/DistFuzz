echo 1024 > /sys/kernel/mm/hugepages/hugepages-2048kB/nr_hugepages

ip l a br0 type bridge
ip l s br0 up

ip l a veth0 type veth peer name br-veth0
ip l a veth1 type veth peer name br-veth1
ip l a veth2 type veth peer name br-veth2

ip l s br-veth0 master br0
ip l s br-veth0 up
ip l s br-veth1 master br0
ip l s br-veth1 up
ip l s br-veth2 master br0
ip l s br-veth2 up

ip a a 10.0.1.4/24 dev veth0
ip a a 10.0.1.5/24 dev veth1
ip a a 10.0.1.6/24 dev veth2
ip l s veth0 up
ip l s veth1 up
ip l s veth2 up

echo 1 > /proc/sys/net/ipv4/conf/veth1/accept_local
echo 1 > /proc/sys/net/ipv4/conf/veth0/accept_local
echo 1 > /proc/sys/net/ipv4/conf/veth2/accept_local
echo 0 > /proc/sys/net/ipv4/conf/all/rp_filter
echo 0 > /proc/sys/net/ipv4/conf/veth0/rp_filter
echo 0 > /proc/sys/net/ipv4/conf/veth1/rp_filter
echo 0 > /proc/sys/net/ipv4/conf/veth2/rp_filter

iptables -A FORWARD -i br0 -j ACCEPT

