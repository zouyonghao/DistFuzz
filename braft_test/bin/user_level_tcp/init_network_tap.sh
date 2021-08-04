# this seems not working
echo 1024 > /sys/kernel/mm/hugepages/hugepages-2048kB/nr_hugepages

ip l a br0 type bridge
ip l s br0 up

tunctl -d tap0
tunctl -d tap1
tunctl -d tap2
tunctl
tunctl
tunctl

ip l s tap0 master br0
ip l s tap1 master br0
ip l s tap2 master br0

ip a a 10.0.1.7/24 dev tap0
ip a a 10.0.1.2/24 dev tap1
ip a a 10.0.1.3/24 dev tap2
ip l s tap0 up
ip l s tap1 up
ip l s tap2 up

echo 1 > /proc/sys/net/ipv4/conf/tap0/accept_local
echo 1 > /proc/sys/net/ipv4/conf/tap1/accept_local
echo 1 > /proc/sys/net/ipv4/conf/tap2/accept_local
echo 0 > /proc/sys/net/ipv4/conf/all/rp_filter
echo 0 > /proc/sys/net/ipv4/conf/tap0/rp_filter
echo 0 > /proc/sys/net/ipv4/conf/tap1/rp_filter
echo 0 > /proc/sys/net/ipv4/conf/tap2/rp_filter

iptables -A FORWARD -i br0 -j ACCEPT

