#ifndef KEEPER_CONFIGURATION_GENERATOR_HEADER
#define KEEPER_CONFIGURATION_GENERATOR_HEADER

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <dst_node_manager.hpp>

#define BIN_PATH "/home/zyh/ClickHouse/build/programs/clickhouse-keeper"
#define BASE_PORT 44444
#define BASE_XML_PATH "enable_keeper.xml"
#define IP "127.0.1.1"

/* The ServerConfigurationGenerator for craft, the example is as follows:
 * clickhouse-keeper --config enable_keeper1.xml
 */
class KeeperConfigurationGenerator : public ServerConfigurationGenerator
{
public:
    void init_xml_file(uint32_t node_id, uint32_t node_count)
    {
        namespace pt = boost::property_tree;
        pt::ptree tree;
        pt::read_xml(BASE_XML_PATH, tree);
        tree.put("yandex.logger.log", "run/run_log" + std::to_string(node_id));
        tree.put("yandex.logger.errorlog", "run/err_log" + std::to_string(node_id));
        tree.put("yandex.keeper_server.server_id", std::to_string(node_id));
        tree.put("yandex.keeper_server.log_storage_path", "run/log" + std::to_string(node_id));
        tree.put("yandex.keeper_server.snapshot_storage_path", "run/snapshot" + std::to_string(node_id));

        pt::ptree servers;
        for (uint32_t i = 0; i < node_count; i++)
        {
            pt::ptree server;
            server.put("id", std::to_string(i));
            server.put("hostname", IP);
            server.put("port", std::to_string(BASE_PORT + i));
            server.put("can_become_leader", "true");
            server.put("priority", "1");
            servers.push_back(std::make_pair("server", server));
        }
        tree.put_child("yandex.keeper_server.raft_configuration", servers);
        pt::write_xml("enable_keeper" + std::to_string(node_id) + ".xml", tree);
    }
    std::string get_configure_string(uint32_t node_id, uint32_t node_count)
    {
        std::string config = BIN_PATH " --config enable_keeper" + std::to_string(node_id) + ".xml";
        init_xml_file(node_id, node_count);
        return config;
    }
};

#endif // KEEPER_CONFIGURATION_GENERATOR_HEADER