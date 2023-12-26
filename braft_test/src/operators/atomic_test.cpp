#include <braft_test_common.hpp>

static ClientConfigurationGenerator *braft_client_configuration_generator = new BraftClientConfigurationGenerator();

/** The InitOperator will try to set until the read is success */
class BraftInitOperator : public DefaultInitOperator
{
public:
    BraftInitOperator() : DefaultInitOperator() {}
    std::string get_write_zero_command() override
    {
        /* TODO: the timeout value should be estimated automatically by running several tests before fuzzing. */
        std::string configure_string = "timeout 3 " CLIENT_BIN_PATH " -conf=";
        for (uint32_t i = 0; i < node_count; i++)
        {
            configure_string += IP ":" + std::to_string(BASE_PORT + i) + ":0,";
        }
        configure_string += " -atomic_op=set -atomic_val=0";
        return configure_string;
    }
};

REGISTER_NORMAL_OPERATOR(AtomicTestGet, new DefaultClientOperator(OP_READ, braft_client_configuration_generator));
REGISTER_NORMAL_OPERATOR(AtomicTestSet, new DefaultClientOperator(OP_WRITE, braft_client_configuration_generator));
REGISTER_NORMAL_OPERATOR(AtomicTestCas, new DefaultClientOperator(OP_CAS, braft_client_configuration_generator));
REGISTER_NORMAL_OPERATOR(Init, new BraftInitOperator);