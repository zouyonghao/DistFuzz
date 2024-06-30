#ifndef DQLITE_CONFIGURATION_GENERATOR_HEADER
#define DQLITE_CONFIGURATION_GENERATOR_HEADER

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <cstdarg>
#include <dst_node_manager.hpp>
#include <operator/dst_default_client_operator.hpp>
#include <operator/dst_default_init_operator.hpp>

#define BIN_PATH "/home/zyh/go/bin/dqlite-demo"
#define BASE_API_PORT 3000
#define BASE_DB_PORT 4000

class DqliteServerConfigurationGenerator : public ServerConfigurationGenerator
{
public:
    void init_data_dir(uint32_t node_id) { system(("mkdir -p ./dqlite_data" + std::to_string(node_id)).c_str()); }
    std::string get_configure_string(uint32_t node_id, uint32_t node_count) override
    {
        std::string config = BIN_PATH " ";
        config += "-D ./dqlite_data" + std::to_string(node_id) + " ";
        config += "-a 127.0.0.1:" + std::to_string(BASE_API_PORT + node_id) + " ";
        config += "-d 127.0.0.1:" + std::to_string(BASE_DB_PORT + node_id) + " ";
        if (node_id > 0) {
            config += "-j 127.0.0.1:" + std::to_string(BASE_DB_PORT);
        }
        init_data_dir(node_id);
        return config;
    }

    std::string get_no_fault_files(uint32_t node_id) override { return ""; }
};

/* The DqliteClientConfigurationGenerator, the example is as follows:
 * curl -X PUT -d my-value http://127.0.0.1:3001/my-key
 * curl http://127.0.0.1:3001/my-key
 */
class DqliteClientConfigurationGenerator : public ClientConfigurationGenerator
{
public:
    std::string get_configure_string(OP_NAME op_name, uint32_t node_count, ...)
    {
        std::string configure_string = "timeout 3 curl ";
        uint32_t node = __dst_get_random_uint8_t() % node_count + BASE_API_PORT;
        va_list random_nums;
        va_start(random_nums, node_count);
        switch (op_name)
        {
        case OP_READ:
            configure_string += "http://127.0.0.1:" + std::to_string(node) + "/my-key";
            break;
        case OP_WRITE:
        {
            uint32_t value = va_arg(random_nums, uint32_t);
            configure_string += "-X PUT -d " + std::to_string(value) + " ";
            configure_string += "http://127.0.0.1:" + std::to_string(node) + "/my-key";
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

#endif // DQLITE_CONFIGURATION_GENERATOR_HEADER