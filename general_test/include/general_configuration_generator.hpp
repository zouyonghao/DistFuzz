#ifndef GENERAL_CONFIGURATION_GENERATOR_HEADER
#define GENERAL_CONFIGURATION_GENERATOR_HEADER

#include <cstdarg>
#include <dst_node_manager.hpp>
#include <operator/dst_default_client_operator.hpp>
#include <operator/dst_default_init_operator.hpp>

class GeneralConfigurationGenerator : public ServerConfigurationGenerator
{
public:
    void init_conf_file(uint32_t node_id, uint32_t node_count) {}

    std::string get_configure_string(uint32_t node_id, uint32_t node_count) override
    {
        std::string config = "";
        std::vector<std::string> args{"-c", "./get_configure.sh", std::to_string(node_id)};
        boost::process::ipstream out;
        boost::process::child c(boost::process::search_path("sh"), args, boost::process::std_out > out);
        for (std::string line; c.running() && std::getline(out, line);)
        {
            config += line;
        }
        c.wait();
        return config;
    }
};

class GeneralClientConfigurationGenerator : public ClientConfigurationGenerator
{
public:
    std::string get_configure_string(OP_NAME op_name, uint32_t node_count, ...)
    {
        std::string configure_string = "sh -c \"./client.sh ";
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
        {
            configure_string += "cas ";
            break;
        }
        default:
            break;
        }
        va_end(random_nums);
        configure_string += "\"";
        return configure_string;
    }
};

#endif // GENERAL_CONFIGURATION_GENERATOR_HEADER