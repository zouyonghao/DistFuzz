mkdir data

for i in 0 1 2 3 4; do
    mkdir -p data/$i
done

bin_path=/home/zyh/etcd

setsid $bin_path/etcd \
  --name node0 \
  --data-dir ./data/0 \
  --heartbeat-interval 300 \
  --election-timeout 1500 \
  --socket-reuse-port \
  --socket-reuse-address \
  --initial-cluster-token etcd-cluster \
  --initial-cluster-state new \
  --initial-advertise-peer-urls http://127.0.1.1:2380 \
  --listen-peer-urls http://127.0.1.1:2380 \
  --listen-client-urls http://127.0.1.1:2480 \
  --advertise-client-urls http://127.0.1.1:2480 \
  --initial-cluster node0=http://127.0.1.1:2380,node1=http://127.0.1.1:2381,node2=http://127.0.1.1:2382,node3=http://127.0.1.1:2383,node4=http://127.0.1.1:2384 \
  > data/log0 2>&1 &

setsid $bin_path/etcd \
  --name node1 \
  --data-dir ./data/1 \
  --heartbeat-interval 300 \
  --election-timeout 1500 \
  --socket-reuse-port \
  --socket-reuse-address \
  --initial-cluster-token etcd-cluster \
  --initial-cluster-state new \
  --initial-advertise-peer-urls http://127.0.1.1:2381 \
  --listen-peer-urls http://127.0.1.1:2381 \
  --listen-client-urls http://127.0.1.1:2481 \
  --advertise-client-urls http://127.0.1.1:2481 \
  --initial-cluster node0=http://127.0.1.1:2380,node1=http://127.0.1.1:2381,node2=http://127.0.1.1:2382,node3=http://127.0.1.1:2383,node4=http://127.0.1.1:2384 \
  > data/log1 2>&1 &

setsid $bin_path/etcd \
  --name node2 \
  --data-dir ./data/2 \
  --heartbeat-interval 300 \
  --election-timeout 1500 \
  --socket-reuse-port \
  --socket-reuse-address \
  --initial-cluster-token etcd-cluster \
  --initial-cluster-state new \
  --initial-advertise-peer-urls http://127.0.1.1:2382 \
  --listen-peer-urls http://127.0.1.1:2382 \
  --listen-client-urls http://127.0.1.1:2482 \
  --advertise-client-urls http://127.0.1.1:2482 \
  --initial-cluster node0=http://127.0.1.1:2380,node1=http://127.0.1.1:2381,node2=http://127.0.1.1:2382,node3=http://127.0.1.1:2383,node4=http://127.0.1.1:2384 \
  > data/log2 2>&1 &

setsid $bin_path/etcd \
  --name node3 \
  --data-dir ./data/3 \
  --heartbeat-interval 300 \
  --election-timeout 1500 \
  --socket-reuse-port \
  --socket-reuse-address \
  --initial-cluster-token etcd-cluster \
  --initial-cluster-state new \
  --initial-advertise-peer-urls http://127.0.1.1:2383 \
  --listen-peer-urls http://127.0.1.1:2383 \
  --listen-client-urls http://127.0.1.1:2483 \
  --advertise-client-urls http://127.0.1.1:2483 \
  --initial-cluster node0=http://127.0.1.1:2380,node1=http://127.0.1.1:2381,node2=http://127.0.1.1:2382,node3=http://127.0.1.1:2383,node4=http://127.0.1.1:2384 \
  > data/log3 2>&1 &

setsid $bin_path/etcd \
  --name node4 \
  --data-dir ./data/4 \
  --heartbeat-interval 300 \
  --election-timeout 1500 \
  --socket-reuse-port \
  --socket-reuse-address \
  --initial-cluster-token etcd-cluster \
  --initial-cluster-state new \
  --initial-advertise-peer-urls http://127.0.1.1:2384 \
  --listen-peer-urls http://127.0.1.1:2384 \
  --listen-client-urls http://127.0.1.1:2484 \
  --advertise-client-urls http://127.0.1.1:2484 \
  --initial-cluster node0=http://127.0.1.1:2380,node1=http://127.0.1.1:2381,node2=http://127.0.1.1:2382,node3=http://127.0.1.1:2383,node4=http://127.0.1.1:2384 \
  > data/log4 2>&1 &
