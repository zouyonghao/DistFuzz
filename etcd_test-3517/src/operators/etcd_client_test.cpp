#include <etcd_configuration_generator.hpp>

static ClientConfigurationGenerator *etcd_client_configuration_generator = new EtcdClientConfigurationGenerator();

/** The InitOperator will try to set until the read is success */
class EtcdInitOperator : public DefaultInitOperator
{
public:
    EtcdInitOperator() : DefaultInitOperator() {}
    std::string get_write_zero_command() override
    {
        std::string configure_string = "timeout 3 /home/zyh/etcd-src/bin/etcdctl --no-sync --timeout 2s --endpoint ";
        for (uint32_t i = 0; i < node_count; i++)
        {
            configure_string += "http://" IP ":" + std::to_string(BASE_CLIENT_PORT + i);
            if (i < node_count - 1)
            {
                configure_string += ",";
            }
        }
        configure_string += " set a 0";
        return configure_string;
    }
};

REGISTER_NORMAL_OPERATOR(EtcdGet, new DefaultClientOperator(OP_READ, etcd_client_configuration_generator));
REGISTER_NORMAL_OPERATOR(EtcdSet, new DefaultClientOperator(OP_WRITE, etcd_client_configuration_generator));
REGISTER_NORMAL_OPERATOR(Init, new EtcdInitOperator);