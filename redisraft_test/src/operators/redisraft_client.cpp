#include <operator/dst_default_client_operator.hpp>
#include <redisraft_configuration_generator.hpp>

static RedisraftClientConfigurationGenerator *rccg = new RedisraftClientConfigurationGenerator();

REGISTER_NORMAL_OPERATOR(RedisraftTestGet, new DefaultClientOperator(OP_READ, rccg));
REGISTER_NORMAL_OPERATOR(RedisraftTestSet, new DefaultClientOperator(OP_WRITE, rccg));

class RedisraftInitOperator : public NormalOperator
{
public:
    int MAX_TRY_COUNT = 5;
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

        int count = 0;
        std::string command = "timeout 3 bash /home/zyh/distributed-system-test/redisraft_test/bin/run_client.sh set 0";
        for (; count < MAX_TRY_COUNT; count++)
        {
            boost::process::child c(command);
            c.wait();
            if (c.exit_code() == 0)
            {
                break;
            }
        }
        if (count >= MAX_TRY_COUNT)
        {
            std::cerr << "Run init failed!\n";
            return false;
        }
        int random_thread_id = random() % INT_MAX;
        std::string invoke_record_string =
            "{:process " + std::to_string(random_thread_id) + ", :type :invoke, :f :write, :value 0}";
        __dst_event_record(invoke_record_string.c_str());
        std::string result_record_string =
            "{:process " + std::to_string(random_thread_id) + ", :type :ok, :f :write, :value 0}";
        __dst_event_record(result_record_string.c_str());
        return true;
    }
};

REGISTER_NORMAL_OPERATOR(Init, new RedisraftInitOperator);