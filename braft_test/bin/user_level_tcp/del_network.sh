ip l d br0
ip l d veth0
ip l d veth1
ip l d veth2

tunctl -d tap0
tunctl -d tap1
tunctl -d tap2

iptables -D FORWARD -i br0 -j ACCEPT

