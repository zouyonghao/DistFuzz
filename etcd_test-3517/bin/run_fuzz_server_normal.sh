bin_path=/home/zyh/etcd-src/bin/

$bin_path/etcd \
  --name node0 \
  --data-dir ./data0 \
  --heartbeat-interval 100 \
  --election-timeout 500 \
  --initial-cluster-token etcd-cluster \
  --initial-cluster-state new \
  --initial-advertise-peer-urls http://127.0.1.1:2380 \
  --listen-peer-urls http://127.0.1.1:2380 \
  --listen-client-urls http://127.0.1.1:2480 \
  --advertise-client-urls http://127.0.1.1:2480 \
  --initial-cluster node0=http://127.0.1.1:2380,node1=http://127.0.1.1:2381,node2=http://127.0.1.1:2382 &

$bin_path/etcd \
  --name node1 \
  --data-dir ./data1 \
  --heartbeat-interval 100 \
  --election-timeout 500 \
  --initial-cluster-token etcd-cluster \
  --initial-cluster-state new \
  --initial-advertise-peer-urls http://127.0.1.1:2381 \
  --listen-peer-urls http://127.0.1.1:2381 \
  --listen-client-urls http://127.0.1.1:2481 \
  --advertise-client-urls http://127.0.1.1:2481 \
  --initial-cluster node0=http://127.0.1.1:2380,node1=http://127.0.1.1:2381,node2=http://127.0.1.1:2382 &

$bin_path/etcd \
  --name node2 \
  --data-dir ./data2 \
  --heartbeat-interval 100 \
  --election-timeout 500 \
  --initial-cluster-token etcd-cluster \
  --initial-cluster-state new \
  --initial-advertise-peer-urls http://127.0.1.1:2382 \
  --listen-peer-urls http://127.0.1.1:2382 \
  --listen-client-urls http://127.0.1.1:2482 \
  --advertise-client-urls http://127.0.1.1:2482 \
  --initial-cluster node0=http://127.0.1.1:2380,node1=http://127.0.1.1:2381,node2=http://127.0.1.1:2382 &
