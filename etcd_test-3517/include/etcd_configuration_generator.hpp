#ifndef ETCD_CONFIGURATION_GENERATOR_HEADER
#define ETCD_CONFIGURATION_GENERATOR_HEADER

#include <cstdarg>
#include <dst_node_manager.hpp>
#include <operator/dst_default_client_operator.hpp>
#include <operator/dst_default_init_operator.hpp>

#define BIN_PATH "/home/zyh/etcd-src/bin/etcd"
#define BASE_ETCD_PORT 2380
#define BASE_CLIENT_PORT 2480
#define IP "127.0.1.1"
#define DATA_DIR_PREFIX "data"

/* The ServerConfigurationGenerator for etcd, the example is as follows:
 * etcd
 * --name node0
 * --data-dir ./data0
 * --heartbeat-interval 100
 * --election-timeout 500
 * --socket-reuse-port
 * --socket-reuse-address
 * --initial-cluster-token etcd-cluster
 * --initial-cluster-state new
 * --initial-advertise-peer-urls http://127.0.1.1:2380
 * --listen-peer-urls http://127.0.1.1:2380
 * --listen-client-urls http://127.0.1.1:2480
 * --advertise-client-urls http://127.0.1.1:2480
 * --initial-cluster node0=http://127.0.1.1:2380,node1=http://127.0.1.1:2381,node2=http://127.0.1.1:2382
 */
class EtcdConfigurationGenerator : public ServerConfigurationGenerator
{
public:
    std::string get_configure_string(uint32_t node_id, uint32_t node_count) override
    {
        std::string node_id_str = std::to_string(node_id);
        std::string config = BIN_PATH;
        config += " --name node" + node_id_str;
        config += " --data-dir ./" DATA_DIR_PREFIX + node_id_str;
        config += " --heartbeat-interval 100";
        config += " --election-timeout 500";
        // config += " --socket-reuse-port";
        // config += " --socket-reuse-address";
        config += " --initial-cluster-token etcd-cluster";
        config += " --initial-cluster-state new";
        config += " --initial-advertise-peer-urls http://" IP ":" + std::to_string(BASE_ETCD_PORT + node_id);
        config += " --listen-peer-urls http://" IP ":" + std::to_string(BASE_ETCD_PORT + node_id);
        config += " --listen-client-urls http://" IP ":" + std::to_string(BASE_CLIENT_PORT + node_id);
        config += " --advertise-client-urls http://" IP ":" + std::to_string(BASE_CLIENT_PORT + node_id);
        config += " --initial-cluster ";
        for (int i = 0; i < node_count; i++)
        {
            config += "node" + std::to_string(i);
            config += "=http://" IP ":" + std::to_string(BASE_ETCD_PORT + i);
            if (i != node_count - 1)
            {
                config += ",";
            }
        }
        return config;
    }
    std::string get_data_folder(uint32_t node_id) override { return DATA_DIR_PREFIX + std::to_string(node_id); }
};

/* The ClientConfigurationGenerator for zookeeper, the example is as follows:
 * /home/zyh/etcd/etcdctl --endpoints=127.0.1.1:2380,127.0.1.1:2381,127.0.1.1:2382 get a
 * /home/zyh/etcd/etcdctl --endpoints=127.0.1.1:2380,127.0.1.1:2381,127.0.1.1:2382 put a 123
 */
class EtcdClientConfigurationGenerator : public ClientConfigurationGenerator
{
public:
    std::string get_configure_string(OP_NAME op_name, uint32_t node_count, ...)
    {
        std::string configure_string = "timeout 3 /home/zyh/etcd-src/bin/etcdctl --no-sync --timeout 2s --endpoint ";
        for (uint32_t i = 0; i < node_count; i++)
        {
            configure_string += "http://" IP ":" + std::to_string(BASE_CLIENT_PORT + i);
            if (i < node_count - 1)
            {
                configure_string += ",";
            }
        }
        configure_string += " ";
        va_list random_nums;
        va_start(random_nums, node_count);
        switch (op_name)
        {
        case OP_READ:
            configure_string += "get a";
            break;
        case OP_WRITE:
        {
            uint32_t value = va_arg(random_nums, uint32_t);
            configure_string += "set a " + std::to_string(value);
            break;
        }
        case OP_CAS:
        {
            break;
        }
        default:
            break;
        }
        va_end(random_nums);
        return configure_string;
    }
};

#endif // ETCD_CONFIGURATION_GENERATOR_HEADER