#include <etcd_configuration_generator.hpp>

static ClientConfigurationGenerator *etcd_client_configuration_generator =
    new EtcdClientConfigurationGenerator();

/** The InitOperator will try to set until the read is success */
class EtcdInitOperator : public NormalOperator
{
public:
    const int MAX_TRY_COUNT = 5;
    EtcdInitOperator() : NormalOperator() {}
    std::string get_command()
    {
        std::string configure_string = "timeout 3 /home/zyh/etcd/bin/etcdctl --endpoints=";
        for (uint32_t i = 0; i < node_count; i++)
        {
            configure_string += IP ":" + std::to_string(BASE_ETCD_PORT + i);
            if (i < node_count - 1)
            {
                configure_string += ",";
            }
        }
        configure_string += " put a 0";
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
            std::cerr << "Etcd init failed!\n";
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

REGISTER_NORMAL_OPERATOR(EtcdGet, new DefaultClientOperator(OP_READ, etcd_client_configuration_generator));
REGISTER_NORMAL_OPERATOR(EtcdSet, new DefaultClientOperator(OP_WRITE, etcd_client_configuration_generator));
REGISTER_NORMAL_OPERATOR(Init, new EtcdInitOperator);