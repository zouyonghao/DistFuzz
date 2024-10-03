#include <etcd_configuration_generator.hpp>

void test_configuration_generator()
{
    EtcdConfigurationGenerator ecg;
    std::cout << ecg.get_configure_string(0, 3) << std::endl;
    std::cout << "/home/zyh/etcd/etcd --name node0 --data-dir ./data0 --heartbeat-interval 100 --election-timeout 500 "
           "--socket-reuse-port --socket-reuse-address --initial-cluster-token etcd-cluster --initial-cluster-state "
           "new --initial-advertise-peer-urls http://127.0.1.1:2380 --listen-peer-urls http://127.0.1.1:2380 "
           "--listen-client-urls http://127.0.1.1:2480 --advertise-client-urls http://127.0.1.1:2480 --initial-cluster "
           "node0=http://127.0.1.1:2380,node1=http://127.0.1.1:2381,node2=http://127.0.1.1:2382" << std::endl;
    assert(ecg.get_configure_string(0, 3) ==
           "/home/zyh/etcd/etcd --name node0 --data-dir ./data0 --heartbeat-interval 100 --election-timeout 500 "
           "--socket-reuse-port --socket-reuse-address --initial-cluster-token etcd-cluster --initial-cluster-state "
           "new --initial-advertise-peer-urls http://127.0.1.1:2380 --listen-peer-urls http://127.0.1.1:2380 "
           "--listen-client-urls http://127.0.1.1:2480 --advertise-client-urls http://127.0.1.1:2480 --initial-cluster "
           "node0=http://127.0.1.1:2380,node1=http://127.0.1.1:2381,node2=http://127.0.1.1:2382");
}

void test_client_configuration_generator()
{
    EtcdClientConfigurationGenerator eccg;
    assert(eccg.get_configure_string(OP_READ, 3) ==
           "timeout 3 /home/zyh/etcd/etcdctl --endpoints=127.0.1.1:2380,127.0.1.1:2381,127.0.1.1:2382 get a");
    assert(eccg.get_configure_string(OP_WRITE, 3, 100) ==
           "timeout 3 /home/zyh/etcd/etcdctl --endpoints=127.0.1.1:2380,127.0.1.1:2381,127.0.1.1:2382 put a 100");
}

int main()
{
    test_configuration_generator();
    test_client_configuration_generator();
}