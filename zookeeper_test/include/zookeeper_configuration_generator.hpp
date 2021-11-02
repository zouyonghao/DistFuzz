#ifndef ZOOKEEPER_CONFIGURATION_GENERATOR_HEADER
#define ZOOKEEPER_CONFIGURATION_GENERATOR_HEADER

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <cstdarg>
#include <dst_node_manager.hpp>
#include <operator/dst_default_client_operator.hpp>

#define BIN_PATH "java -cp /home/zyh/zookeeper/lib/zookeeper-3.7.0.jar:/home/zyh/zookeeper/lib/*:/home/zyh/zookeeper/conf org.apache.zookeeper.server.quorum.QuorumPeerMain "
#define BASE_ZK_PORT 2181
#define BASE_QUORUM_PORT 2888
#define BASE_ELECTION_PORT 3888
#define BASE_CONF_PATH_PREFIX "zoo"
#define IP "127.0.1.1"
#define DATA_DIR_PREFIX "data"

/* The ServerConfigurationGenerator for zk, the example is as follows:
 * java -cp zookeeper/lib/*:zookeeper/conf org.apache.zookeeper.server.quorum.QuorumPeerMain zoo1
 */
class ZooKeeperConfigurationGenerator : public ServerConfigurationGenerator
{
public:
    void init_conf_file(uint32_t node_id, uint32_t node_count)
    {
        /** zk server id starts from 1 */
        std::string zk_id_str = std::to_string(node_id + 1);
        std::ofstream zk_conf_file(BASE_CONF_PATH_PREFIX + zk_id_str);
        zk_conf_file << "tickTime=200\n";
        zk_conf_file << "initLimit=5\n";
        zk_conf_file << "syncLimit=2\n";
        zk_conf_file << "admin.enableServer=false\n";
        zk_conf_file << "dataDir=" DATA_DIR_PREFIX << zk_id_str << "\n";
        zk_conf_file << "clientPort=" << std::to_string(BASE_ZK_PORT + node_id) << "\n";
        for (int i = 0; i < node_count; i++)
        {
            zk_conf_file << "server." << std::to_string(i + 1) << "=" IP ":" << std::to_string(BASE_QUORUM_PORT + i)
                         << ":" << std::to_string(BASE_ELECTION_PORT + i) << "\n";
        }
        zk_conf_file.close();
    }

    void init_data_folder(uint32_t node_id)
    {
        /** zk server id starts from 1 */
        std::string zk_id_str = std::to_string(node_id + 1);
        std::system(("mkdir " DATA_DIR_PREFIX + zk_id_str).c_str());
        std::system(("bash -c \"echo " + zk_id_str + " > " DATA_DIR_PREFIX + zk_id_str + "/myid\"").c_str());
    }
    std::string get_configure_string(uint32_t node_id, uint32_t node_count)
    {
        init_conf_file(node_id, node_count);
        init_data_folder(node_id);
        std::string config = BIN_PATH BASE_CONF_PATH_PREFIX + std::to_string(node_id + 1);
        return config;
    }
};

/* The ClientConfigurationGenerator for zookeeper, the example is as follows:
 * /usr/share/zookeeper/bin/zkCli.sh -server 127.0.1.1:9181,127.0.1.1:9182,127.0.1.1:9183 get /a
 * /usr/share/zookeeper/bin/zkCli.sh -server 127.0.1.1:9181,127.0.1.1:9182,127.0.1.1:9183 set /a 1
 */
class ZooKeeperClientConfigurationGenerator : public ClientConfigurationGenerator
{
public:
    std::string get_configure_string(OP_NAME op_name, uint32_t node_count, ...)
    {
        std::string configure_string = "timeout 3 /usr/share/zookeeper/bin/zkCli.sh -server ";
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
        switch (op_name)
        {
        case OP_READ:
            configure_string += "get /a";
            break;
        case OP_WRITE:
        {
            uint32_t value = va_arg(random_nums, uint32_t);
            configure_string += "set /a " + std::to_string(value);
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

class ZooKeeperClientWriteWithVersionConfigurationGenerator : public ClientConfigurationGenerator
{
public:
    std::string get_configure_string(OP_NAME op_name, uint32_t node_count, ...)
    {
        std::string configure_string = "timeout 3 /usr/share/zookeeper/bin/zkCli.sh -server ";
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
        switch (op_name)
        {
        case OP_WRITE:
        {
            uint32_t value = va_arg(random_nums, uint32_t);
            /** The version should not be too large */
            uint32_t version = __dst_get_random_uint8_t() % 3;
            configure_string += "set /a " + std::to_string(value) + " " + std::to_string(version);
            break;
        }
        default:
            break;
        }
        va_end(random_nums);
        return configure_string;
    }
};

#endif // ZOOKEEPER_CONFIGURATION_GENERATOR_HEADER