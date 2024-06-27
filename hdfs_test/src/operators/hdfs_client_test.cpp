#include <hdfs_configuration_generator.hpp>


static ClientConfigurationGenerator *hdfs_client_configuration_generator =
    new HdfsClientConfigurationGenerator();

class HdfsClientOperator : public DefaultClientOperator
{
public:
    HdfsClientOperator(OP_NAME op_name, ClientConfigurationGenerator *ccg) : DefaultClientOperator(op_name, ccg) {}

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

REGISTER_NORMAL_OPERATOR(HdfsGet, new HdfsClientOperator(OP_READ, hdfs_client_configuration_generator));
REGISTER_NORMAL_OPERATOR(HdfsSet, new HdfsClientOperator(OP_WRITE, hdfs_client_configuration_generator));

REGISTER_NORMAL_OPERATOR(Init, new HdfsClientOperator(OP_WRITE, hdfs_client_configuration_generator));