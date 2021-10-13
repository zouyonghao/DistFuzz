#include <nuraft_configuration_generator.hpp>

static NuraftClientConfigurationGenerator *nccg = new NuraftClientConfigurationGenerator();

REGISTER_NORMAL_OPERATOR(NuraftTestGet, new DefaultClientOperator(OP_READ, nccg));
REGISTER_NORMAL_OPERATOR(NuraftTestSet, new DefaultClientOperator(OP_WRITE, nccg));

class NuraftInitOperator : public NormalOperator
{
public:
    bool _do() override
    {
        for (int i = 1; i < node_count; i++)
        {
            usleep(500000);
            std::string command =
                "echo add " + std::to_string(i + 1) + " " IP ":" + std::to_string(BASE_PORT + i) + " > calc1.pipe";
            std::cerr << "init command is " << command << "\n";
            system(command.c_str());
        }
        return true;
    }
};

REGISTER_NORMAL_OPERATOR(Init, new NuraftInitOperator);