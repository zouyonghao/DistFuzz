#ifndef ZOOKEEPER_CONFIGURATION_GENERATOR_HEADER
#define ZOOKEEPER_CONFIGURATION_GENERATOR_HEADER

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <cstdarg>
#include <dst_node_manager.hpp>
#include <operator/dst_default_client_operator.hpp>
#include <operator/dst_default_init_operator.hpp>

#define BIN_PATH                                                                                                       \
    "java -cp "                                                                                                        \
    "/home/zouyo/hadoop:/home/zouyo/hadoop/share/hadoop/common/*:/home/zouyo/hadoop/share/hadoop/common/lib/*:/home/"  \
    "zouyo/hadoop/share/hadoop/hdfs/*:/home/zouyo/hadoop/share/hadoop/hdfs/lib/*:./datanode"

class HdfsConfigurationGenerator : public ServerConfigurationGenerator
{
public:
    std::string get_configure_string(uint32_t node_id, uint32_t node_count) override
    {
        std::string config = BIN_PATH + std::to_string(node_id + 1) + " org.apache.hadoop.hdfs.server.datanode.DataNode";
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

#endif // ZOOKEEPER_CONFIGURATION_GENERATOR_HEADER