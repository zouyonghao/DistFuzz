#ifndef HDFS_CONFIGURATION_GENERATOR_HEADER
#define HDFS_CONFIGURATION_GENERATOR_HEADER

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <cstdarg>
#include <dst_node_manager.hpp>
#include <operator/dst_default_client_operator.hpp>
#include <operator/dst_default_init_operator.hpp>

#define HADOOP_PATH "/home/zyh/hadoop"

#define BIN_PATH                                                                                                       \
    "java -cp " HADOOP_PATH ":" HADOOP_PATH "/share/hadoop/common/*:" HADOOP_PATH                                      \
    "/share/hadoop/common/lib/*:" HADOOP_PATH "/share/hadoop/hdfs/*:" HADOOP_PATH                                      \
    "/share/hadoop/hdfs/lib/*:./datanode"

class HdfsConfigurationGenerator : public ServerConfigurationGenerator
{
public:
    std::string get_configure_string(uint32_t node_id, uint32_t node_count) override
    {
        std::string config =
            BIN_PATH + std::to_string(node_id + 1) + " org.apache.hadoop.hdfs.server.datanode.DataNode";
        return config;
    }
    std::string get_data_folder(uint32_t node_id) override { return "hadoop/datanode" + std::to_string(node_id + 1); }

    /* exclude config file, myid, and snapshot files(too many errors). */
    std::string get_no_fault_files(uint32_t node_id) override
    {
        std::string no_fault_files = "";
        return no_fault_files;
    }
};

/* The ClientConfigurationGenerator for zookeeper, the example is as follows:
 * echo 1 > a
 * hadoop/bin/hdfs dfs -put ./a /a
 * hadoop/bin/hdfs dfs -cat /a
 */
class HdfsClientConfigurationGenerator : public ClientConfigurationGenerator
{
public:
    std::string get_configure_string(OP_NAME op_name, uint32_t node_count, ...)
    {
        std::string configure_string = "timeout 3 " HADOOP_PATH "/bin/hdfs dfs ";
        if (__dst_get_random_uint8_t() > 128)
        {
            // use namenode2
            configure_string += "-fs hdfs://localhost:9001 ";
        }
        va_list random_nums;
        va_start(random_nums, node_count);
        switch (op_name)
        {
        case OP_READ:
            configure_string += "-cat /a";
            break;
        case OP_WRITE:
        {
            uint32_t value = va_arg(random_nums, uint32_t);
            boost::filesystem::ofstream a("./a");
            a << value;
            a.close();
            configure_string += "-put ./a /a";
            break;
        }
        case OP_CAS:
        {
            break;
        }
        default:
            break;
        }
        va_end(random_nums);
        return configure_string;
    }
};

#endif // HDFS_CONFIGURATION_GENERATOR_HEADER