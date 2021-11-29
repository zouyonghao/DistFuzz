#include <operator/dst_default_client_operator.hpp>
#include <redisraft_configuration_generator.hpp>

static RedisraftClientConfigurationGenerator *rccg = new RedisraftClientConfigurationGenerator();

REGISTER_NORMAL_OPERATOR(RedisraftTestGet, new DefaultClientOperator(OP_READ, rccg));
REGISTER_NORMAL_OPERATOR(RedisraftTestSet, new DefaultClientOperator(OP_WRITE, rccg));

class RedisraftInitOperator : public NormalOperator
{
public:
    bool _do() override
    {
        sleep(1);
        system(("redis-cli -h " IP " -p " + std::to_string(BASE_PORT) + " raft.cluster init").c_str());
        for (int i = 1; i < node_count; i++)
        {
            std::string command = "timeout 2 redis-cli -h " IP " -p " + std::to_string(BASE_PORT + i) +
                                  " RAFT.CLUSTER JOIN " IP ":" + std::to_string(BASE_PORT);
            std::cerr << "init command is " << command << "\n";
            system(command.c_str());
        }
        return true;
    }
};

REGISTER_NORMAL_OPERATOR(Init, new RedisraftInitOperator);