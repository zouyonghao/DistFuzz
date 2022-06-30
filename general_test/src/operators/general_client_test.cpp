#include <general_configuration_generator.hpp>

static ClientConfigurationGenerator *general_client_configuration_generator = new GeneralClientConfigurationGenerator();

class GeneralClientOperator : public DefaultClientOperator
{
public:
    GeneralClientOperator(OP_NAME op_name, ClientConfigurationGenerator *ccg) : DefaultClientOperator(op_name, ccg) {}
};

class GeneralInitOperator : public DefaultInitOperator
{
public:
    GeneralInitOperator() : DefaultInitOperator(10) {}
    std::string get_write_zero_command()
    {
        std::string configure_string = "sh -c ./client.sh init";
        return configure_string;
    }
};

REGISTER_NORMAL_OPERATOR(GeneralGet, new GeneralClientOperator(OP_READ, general_client_configuration_generator));
REGISTER_NORMAL_OPERATOR(GeneralSet, new GeneralClientOperator(OP_WRITE, general_client_configuration_generator));
REGISTER_NORMAL_OPERATOR(GeneralCas, new GeneralClientOperator(OP_CAS, general_client_configuration_generator));
REGISTER_NORMAL_OPERATOR(Init, new GeneralInitOperator);