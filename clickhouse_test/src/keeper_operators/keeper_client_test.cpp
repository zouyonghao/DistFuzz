#include <keeper_configuration_generator.hpp>

static ClientConfigurationGenerator *keeper_client_configuration_generator = new KeeperClientConfigurationGenerator();

/** The KeeperInitOperator will try to set until the read is success */
class KeeperInitOperator : public DefaultClientOperator
{
public:
    const int MAX_TRY_COUNT = 1000;
    KeeperInitOperator() : DefaultClientOperator(OP_WRITE, keeper_client_configuration_generator) {}

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