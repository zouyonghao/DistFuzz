#include <etcd_configuration_generator.hpp>

void test_configuration_generator()
{
    EtcdConfigurationGenerator ecg;
    std::cout << ecg.get_configure_string(0, 3) << std::endl;
    std::cout << "/home/zyh/etcd-src/bin/etcd --name node0 --data-dir ./data0 --heartbeat-interval 100 --election-timeout 500 "
           "--initial-cluster-token etcd-cluster --initial-cluster-state "
           "new --initial-advertise-peer-urls http://127.0.1.1:2380 --listen-peer-urls http://127.0.1.1:2380 "
           "--listen-client-urls http://127.0.1.1:2480 --advertise-client-urls http://127.0.1.1:2480 --initial-cluster "
           "node0=http://127.0.1.1:2380,node1=http://127.0.1.1:2381,node2=http://127.0.1.1:2382" << std::endl;
    assert(ecg.get_configure_string(0, 3) ==
           "/home/zyh/etcd-src/bin/etcd --name node0 --data-dir ./data0 --heartbeat-interval 100 --election-timeout 500 "
           "--initial-cluster-token etcd-cluster --initial-cluster-state "
           "new --initial-advertise-peer-urls http://127.0.1.1:2380 --listen-peer-urls http://127.0.1.1:2380 "
           "--listen-client-urls http://127.0.1.1:2480 --advertise-client-urls http://127.0.1.1:2480 --initial-cluster "
           "node0=http://127.0.1.1:2380,node1=http://127.0.1.1:2381,node2=http://127.0.1.1:2382");
}

void test_client_configuration_generator()
{
    EtcdClientConfigurationGenerator eccg;
    std::cout << eccg.get_configure_string(OP_READ, 3) << std::endl;
    assert(eccg.get_configure_string(OP_READ, 3) ==
           "timeout 3 /home/zyh/etcd-src/bin/etcdctl --no-sync --timeout 2s --endpoint http://127.0.1.1:2480,http://127.0.1.1:2481,http://127.0.1.1:2482 get a");
    std::cout << eccg.get_configure_string(OP_WRITE, 3, 100) << std::endl;
    assert(eccg.get_configure_string(OP_WRITE, 3, 100) ==
           "timeout 3 /home/zyh/etcd-src/bin/etcdctl --no-sync --timeout 2s --endpoint http://127.0.1.1:2480,http://127.0.1.1:2481,http://127.0.1.1:2482 set a 100");
}

int main()
{
    test_configuration_generator();
    test_client_configuration_generator();
}