#ifndef DST_COMMON_H
#define DST_COMMON_H

#include <cstdarg>
#include <operator/dst_default_client_operator.hpp>
#include <operator/dst_default_init_operator.hpp>

#define BIN_PATH "/home/zyh/braft/example/atomic/atomic_server"
#define BASE_PORT 8300
#define IP "127.0.1.1"

/* The ServerConfigurationGenerator for braft, the example is as follows:
 * atomic_server -reuse_addr -ip=127.0.1.1 -reuse_port -port=8300 -election_timeout_ms=200
 * -conf="127.0.1.1:8300:0,127.0.1.1:8301:0,127.0.1.1:8302:0," -data_path="./data0
 */
class BraftConfigurationGenerator : public ServerConfigurationGenerator
{
public:
    std::string get_configure_string(uint32_t node_id, uint32_t node_count) override
    {
        std::string config = BIN_PATH " -reuse_addr -ip=" IP " -reuse_port -election_timeout_ms=500 ";
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

/* The ClientConfigurationGenerator for braft, the example is as follows:
 * atomic_test -conf="127.0.1.1:8300:0,127.0.1.1:8301:0,127.0.1.1:8302:0," -atomic_op=get
 * atomic_test -conf="127.0.1.1:8300:0,127.0.1.1:8301:0,127.0.1.1:8302:0," -atomic_op=set -atomic_val=1
 * atomic_test -conf="127.0.1.1:8300:0,127.0.1.1:8301:0,127.0.1.1:8302:0," -atomic_op=cas -atomic_val=1
 * -atomic_new_val=2
 */
class BraftClientConfigurationGenerator : public ClientConfigurationGenerator
{
public:
    std::string get_configure_string(OP_NAME op_name, uint32_t node_count, ...) override
    {
        /* TODO: the timeout value should be estimated automatically by running several tests before fuzzing. */
        std::string configure_string = "timeout 3 /home/zyh/braft/example/atomic/atomic_test -conf=";
        for (uint32_t i = 0; i < node_count; i++)
        {
            configure_string += IP ":" + std::to_string(BASE_PORT + i) + ":0,";
        }
        configure_string += " -atomic_op=";
        va_list random_nums;
        va_start(random_nums, node_count);
        switch (op_name)
        {
        case OP_READ:
            configure_string += "get";
            break;
        case OP_WRITE:
        {
            uint32_t value = va_arg(random_nums, uint32_t);
            configure_string += "set -atomic_val=" + std::to_string(value);
            break;
        }
        case OP_CAS:
        {
            uint32_t old_value = va_arg(random_nums, uint32_t);
            uint32_t new_value = va_arg(random_nums, uint32_t);
            configure_string +=
                "cas -atomic_val=" + std::to_string(old_value) + " -atomic_new_val=" + std::to_string(new_value);
            break;
        }
        default:
            break;
        }
        va_end(random_nums);
        return configure_string;
    }
};

#endif // DST_COMMON_H