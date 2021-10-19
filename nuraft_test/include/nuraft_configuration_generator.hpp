#ifndef NURAFT_CONFIGURATION_GENERATOR_HEADER
#define NURAFT_CONFIGURATION_GENERATOR_HEADER

#include <cstdarg>
#include <dst_node_manager.hpp>
#include <operator/dst_default_client_operator.hpp>

#define SERVER_BIN_PATH "/home/zyh/NuRaft/build/examples/calc_server"
#define BASE_PORT 10001
#define IP "127.0.1.1"

/* The ServerConfigurationGenerator for nuraft, the example is as follows:
 * calc_server 1 127.0.1.1:10001
 */
class NuraftConfigurationGenerator : public ServerConfigurationGenerator
{
public:
    std::string get_configure_string(uint32_t node_id, uint32_t node_count)
    {
        std::string config =
            SERVER_BIN_PATH " " + std::to_string(node_id + 1) + " " IP ":" + std::to_string(BASE_PORT + node_id);
        return config;
    }
};

/* The ClientConfigurationGenerator for NuRaft calc_server, the example is as follows:
 * bash /home/zyh/distributed-system-test/nuraft_test/bin/run_client.sh get
 * bash /home/zyh/distributed-system-test/nuraft_test/bin/run_client.sh set 1
 */
class NuraftClientConfigurationGenerator : public ClientConfigurationGenerator
{
public:
    std::string get_configure_string(OP_NAME op_name, uint32_t node_count, ...)
    {
        std::string configure_string = "timeout 2 bash /home/zyh/distributed-system-test/nuraft_test/bin/run_client.sh ";
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
            configure_string += "set " + std::to_string(value);
            break;
        }
        case OP_CAS:
            /* currently not supported */
            configure_string = "";
            break;
        default:
            break;
        }
        va_end(random_nums);
        return configure_string;
    }
};

#endif // NURAFT_CONFIGURATION_GENERATOR_HEADER