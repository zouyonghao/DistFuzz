#ifndef DST_COMMON_H
#define DST_COMMON_H
#include <operator/dst_default_client_operator.hpp>

#define BIN_PATH "/home/zyh/braft/example/atomic/atomic_server"
#define BASE_PORT 8300
#define IP "127.0.1.1"

/* The ServerConfigurationGenerator for braft, the example is as follows:
 * atomic_server -reuse_addr -ip=127.0.1.1 -reuse_port -port=8300 -election_timeout_ms=200
 * -conf="127.0.1.1:8300:0,127.0.1.1:8301:0,127.0.1.1:8302:0," -data_path="./data0
 */
class BraftConfigurationGenerator : public ServerConfigurationGenerator
{
    std::string get_configure_string(uint32_t node_id, uint32_t node_count)
    {
        std::string config = BIN_PATH " -reuse_addr -ip=" IP " -reuse_port -election_timeout_ms=200 ";
        config += "-port=" + std::to_string(BASE_PORT + node_id) + " ";
        config += "-conf=";
        for (uint32_t i = 0; i < node_count; i++)
        {
            config += IP ":" + std::to_string(BASE_PORT + i) + ":0,";
        }
        config += " ";
        config += "-data_path=./data" + std::to_string(node_id);
        return config;
    }
};

class BraftClientConfigurationGenerator : public ClientConfigurationGenerator
{
public:
    std::string get_read_configure_string(uint32_t node_count) { return get_configure_string(node_count) + "get"; }
    std::string get_write_configure_string(uint32_t node_count, uint32_t value)
    {
        return get_configure_string(node_count) + "set -atomic_val=" + std::to_string(value);
    }
    std::string get_cas_configure_string(uint32_t node_count, uint32_t old_value, uint32_t new_value)
    {
        return get_configure_string(node_count) + "cas -atomic_val=" + std::to_string(old_value) +
               " -atomic_new_val=" + std::to_string(new_value);
    }

private:
    std::string get_configure_string(uint32_t node_count)
    {
        std::string configure_string = "timeout 2 /home/zyh/braft/example/atomic/atomic_test -conf=";
        for (uint32_t i = 0; i < node_count; i++)
        {
            configure_string += IP ":" + std::to_string(BASE_PORT + i) + ":0,";
        }
        configure_string += " -atomic_op=";
        return configure_string;
    }
};

#endif // DST_COMMON_H