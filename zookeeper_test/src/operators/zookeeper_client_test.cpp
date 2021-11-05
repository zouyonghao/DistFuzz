#include <zookeeper_configuration_generator.hpp>

static ClientConfigurationGenerator *zookeeper_client_configuration_generator =
    new ZooKeeperClientConfigurationGenerator();

/** The KeeperInitOperator will try to set until the read is success */
class ZooKeeperInitOperator : public NormalOperator
{
public:
    const int MAX_TRY_COUNT = 20;
    ZooKeeperInitOperator() : NormalOperator() {}
    std::string get_command()
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
        configure_string += " create /a 0";
        return configure_string;
    }

    bool _do() override
    {
        int count = 0;
        while (count < MAX_TRY_COUNT)
        {
            sleep(1);
            boost::process::child c(get_command());
            c.wait();
            if (c.exit_code() == 0)
            {
                break;
            }
            count++;
        }
        if (count >= MAX_TRY_COUNT)
        {
            std::cerr << "Keeper init failed!\n";
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

class ZooKeeperClientOperator : public DefaultClientOperator
{
public:
    ZooKeeperClientOperator(OP_NAME op_name, ClientConfigurationGenerator *ccg) : DefaultClientOperator(op_name, ccg) {}

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

REGISTER_NORMAL_OPERATOR(ZooKeeperGet, new ZooKeeperClientOperator(OP_READ, zookeeper_client_configuration_generator));
REGISTER_NORMAL_OPERATOR(ZooKeeperSet, new ZooKeeperClientOperator(OP_WRITE, zookeeper_client_configuration_generator));
REGISTER_NORMAL_OPERATOR(ZooKeeperSetWithVersion,
                         new ZooKeeperClientOperator(OP_WRITE,
                                                     new ZooKeeperClientWriteWithVersionConfigurationGenerator));
REGISTER_NORMAL_OPERATOR(Init, new ZooKeeperInitOperator);