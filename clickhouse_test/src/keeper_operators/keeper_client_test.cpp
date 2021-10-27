#include <keeper_configuration_generator.hpp>

static ClientConfigurationGenerator *keeper_client_configuration_generator = new KeeperClientConfigurationGenerator();

/** The KeeperInitOperator will try to set until the read is success */
class KeeperInitOperator : public DefaultClientOperator
{
    class InitConfigurationGenerator : public ClientConfigurationGenerator
    {
    public:
        std::string get_configure_string(OP_NAME op_name, uint32_t node_count, ...)
        {
            std::string configure_string = "timeout 2 /usr/share/zookeeper/bin/zkCli.sh -server ";
            for (uint32_t i = 0; i < node_count; i++)
            {
                configure_string += IP ":" + std::to_string(BASE_ZK_PORT + i);
                if (i < node_count - 1)
                {
                    configure_string += ",";
                }
            }
            configure_string += " ";
            va_list random_nums;
            va_start(random_nums, node_count);
            uint32_t value = va_arg(random_nums, uint32_t);
            configure_string += "create /a " + std::to_string(value);
            va_end(random_nums);
            return configure_string;
        }
    };

public:
    const int MAX_TRY_COUNT = 1000;
    KeeperInitOperator() : DefaultClientOperator(OP_WRITE, new InitConfigurationGenerator) {}

    bool _do() override
    {
        int count = 0;
        while (count < MAX_TRY_COUNT)
        {
            sleep(1);
            if (DefaultClientOperator::_do())
            {
                break;
            }
        }
        if (count >= MAX_TRY_COUNT)
        {
            std::cerr << "Keeper init failed!\n";
            return false;
        }
        return true;
    }
};

REGISTER_NORMAL_OPERATOR(KeeperGet, new DefaultClientOperator(OP_READ, keeper_client_configuration_generator));
REGISTER_NORMAL_OPERATOR(KeeperSet, new DefaultClientOperator(OP_WRITE, keeper_client_configuration_generator));
REGISTER_NORMAL_OPERATOR(Init, new KeeperInitOperator);