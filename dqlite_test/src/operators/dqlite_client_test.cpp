#include <dqlite_configuration_generator.hpp>


static DqliteClientConfigurationGenerator *dqlite_client_configuration_generator =
    new DqliteClientConfigurationGenerator();

class DqliteClientOperator : public DefaultClientOperator
{
public:
    DqliteClientOperator(OP_NAME op_name, ClientConfigurationGenerator *ccg) : DefaultClientOperator(op_name, ccg) {}

    int64_t parse_read_result(boost::process::ipstream &pipe_stream) override
    {
        /* the last output should be the result of read */
        std::string last_output;
        std::string tmp;
        while (pipe_stream && std::getline(pipe_stream, tmp))
        {
            std::cerr << tmp << "\n";
            last_output = tmp;
        }
        return std::stoll(last_output);
    }
};


class DqliteInitOperator : public DefaultInitOperator
{
public:
    DqliteInitOperator() : DefaultInitOperator(5) {}

    std::string get_write_zero_command()
    {
        std::string configure_string = "timeout 3 curl ";
        uint32_t node = __dst_get_random_uint8_t() % node_count + BASE_API_PORT;
        configure_string += "-X PUT -d 0 ";
        configure_string += "http://127.0.0.1:" + std::to_string(node) + "/my-key";
        return configure_string;
    }
};

REGISTER_NORMAL_OPERATOR(DqliteGet, new DqliteClientOperator(OP_READ, dqlite_client_configuration_generator));
REGISTER_NORMAL_OPERATOR(DqliteSet, new DqliteClientOperator(OP_WRITE, dqlite_client_configuration_generator));

REGISTER_NORMAL_OPERATOR(Init, new DqliteInitOperator);