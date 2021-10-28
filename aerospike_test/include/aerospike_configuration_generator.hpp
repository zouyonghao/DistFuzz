#ifndef AEROSPIKE_CONFIGURATION_GENERATOR_HEADER
#define AEROSPIKE_CONFIGURATION_GENERATOR_HEADER

#include <dst_node_manager.hpp>

#include <fstream>
#include <iostream>

#define BIN_PATH "/home/zyh/aerospike-server/target/Linux-x86_64/bin/asd"

#define CONFIG_FILE_PIECE_SERVICE_1 "service {\n\tuser zyh\n\tgroup zyh\n\tpaxos-single-replica-limit 0\n"
/* Add pidfile, work-directory */
#define PID_FILE_PREFIX "\tpidfile /home/zyh/distributed-system-test/aerospike_test/bin/run/asd_pid"
#define WORK_DIRECTORY_PREFIX "\twork-directory run/work"
#define CONFIG_FILE_PIECE_SERVICE_2 "\n\tproto-fd-max 1500\n\trun-as-daemon false\n}\n"

#define CONFIG_FILE_PIECE_MOD_1 "\nmod-lua {\n"
/* Add user-path */
#define USER_PATH_PREFIX "\tuser-path run/work"
#define USER_PATH_SUFFIX "/usr/udf/lua\n"
#define CONFIG_FILE_PIECE_MOD_2 "}\n"

#define CONFIG_FILE_PIECE_LOGGING_1 "\nlogging {\n"
/* Add file */
#define FILE_PREFIX "\tfile /home/zyh/distributed-system-test/aerospike_test/bin/run/log"
#define CONFIG_FILE_PIECE_LOGGING_2 " {\n\t\tcontext any info\n\t}\n}\n"

#define CONFIG_FILE_PIECE_NETWORK_1 "\nnetwork {\n\tservice {\n\t\taddress 127.0.1.1\n\taccess-address 127.0.1.1\n"
/* Add service port */
#define SERVICE_PORT_PREFIX "\t\tport "
#define SERVICE_BASE_PORT 2000
#define CONFIG_FILE_PIECE_NETWORK_2 "\t}\n"

#define CONFIG_FILE_PIECE_NETWORK_3 "\n\theartbeat {\n\t\tmode mesh\n\t\taddress 127.0.1.1\n"
/* Add port, multiple mesh-seed-address-port */
#define HEARTBEAT_PORT_PREFIX "\t\tport "
#define HEARTBEAT_BASE_PORT 5000
#define MESH_PORT_PREFIX "\t\tmesh-seed-address-port 127.0.1.1 "
#define CONFIG_FILE_PIECE_NETWORK_4 "\n\t\tinterval 100\n\t\ttimeout 3\n\t\tconnect-timeout-ms 100\n\t}\n"

/* Add a address to fabric to avoid error 1 */
#define CONFIG_FILE_PIECE_NETWORK_5 "\n\tfabric {\n\t\taddress 127.0.1.1\n"
/* Add fabric port */
#define FABRIC_PORT_PREFIX "\t\tport "
#define FABRIC_BASE_PORT 4000
#define CONFIG_FILE_PIECE_NETWORK_6 "\t}\n"

#define CONFIG_FILE_PIECE_NETWORK_7 "\tinfo {\n"
/* Add info port */
#define INFO_PORT_PREFIX "\t\tport "
#define INFO_BASE_PORT 3000
#define CONFIG_FILE_PIECE_NETWORK_8 "\n\t}\n}\n"

#define CONFIG_FILE_PIECE_NAMESPACE_1                                                                                  \
    "\nnamespace test {\n\treplication-factor 2\n\tmemory-size 1G\n\n\tstorage-engine device {\n"
/* Add data file */
#define STORAGE_FILE_PREFIX "\t\tfile run/data"
#define CONFIG_FILE_PIECE_NAMESPACE_2 "\t\tfilesize 10M\n\t\tdata-in-memory true\n\t}\n}\n"

/* The ServerConfigurationGenerator for aerospike, the example is as follows:
 * asd --config-file aerospike_mesh1.conf
 */
class AerospikeConfigurationGenerator : public ServerConfigurationGenerator
{
public:
    static void init_config_file(uint32_t node_id, uint32_t node_count)
    {
        std::string node_id_str = std::to_string(node_id);
        std::string file_content = CONFIG_FILE_PIECE_SERVICE_1;
        file_content += PID_FILE_PREFIX + node_id_str + "\n";
        file_content += WORK_DIRECTORY_PREFIX + node_id_str + "\n";
        file_content += CONFIG_FILE_PIECE_SERVICE_2;

        file_content += CONFIG_FILE_PIECE_MOD_1;
        file_content += USER_PATH_PREFIX + node_id_str + USER_PATH_SUFFIX;
        file_content += CONFIG_FILE_PIECE_MOD_2;

        file_content += CONFIG_FILE_PIECE_LOGGING_1;
        file_content += FILE_PREFIX + node_id_str;
        file_content += CONFIG_FILE_PIECE_LOGGING_2;

        file_content += CONFIG_FILE_PIECE_NETWORK_1;
        file_content += SERVICE_PORT_PREFIX + std::to_string(SERVICE_BASE_PORT + node_id) + "\n";
        file_content += CONFIG_FILE_PIECE_NETWORK_2;

        file_content += CONFIG_FILE_PIECE_NETWORK_3;
        file_content += HEARTBEAT_PORT_PREFIX + std::to_string(HEARTBEAT_BASE_PORT + node_id) + "\n";

        for (int i = 0; i < node_count; i++)
        {
            if (i == node_id)
            {
                continue;
            }
            file_content += MESH_PORT_PREFIX + std::to_string(HEARTBEAT_BASE_PORT + i) + "\n";
        }

        file_content += CONFIG_FILE_PIECE_NETWORK_4;

        file_content += CONFIG_FILE_PIECE_NETWORK_5;
        file_content += FABRIC_PORT_PREFIX + std::to_string(FABRIC_BASE_PORT + node_id) + "\n";
        file_content += CONFIG_FILE_PIECE_NETWORK_6;

        file_content += CONFIG_FILE_PIECE_NETWORK_7;
        file_content += INFO_PORT_PREFIX + std::to_string(INFO_BASE_PORT + node_id) + "\n";
        file_content += CONFIG_FILE_PIECE_NETWORK_8;

        file_content += CONFIG_FILE_PIECE_NAMESPACE_1;
        file_content += STORAGE_FILE_PREFIX + node_id_str + "\n";
        file_content += CONFIG_FILE_PIECE_NAMESPACE_2;

        std::ofstream config_file;
        config_file.open("aerospike_conf" + node_id_str);
        config_file << file_content;
        config_file.close();
    }

    static void init_working_dir(uint32_t node_id)
    {
        std::string node_id_str = std::to_string(node_id);
        // system(("mkdir -p run/log" + node_id_str).c_str());
        system(("mkdir -p run/work" + node_id_str + "/smd").c_str());
        system(("mkdir -p run/work" + node_id_str + "/sys/udf/lua").c_str());
        system(("mkdir -p run/work" + node_id_str + "/usr/udf/lua").c_str());
    }

    std::string get_configure_string(uint32_t node_id, uint32_t node_count) override
    {
        /** TODO: should we do this operation here? */
        init_config_file(node_id, node_count);
        init_working_dir(node_id);
        std::string config = BIN_PATH " ";
        config += "--config-file aerospike_conf" + std::to_string(node_id);
        return config;
    }
};

#endif // AEROSPIKE_CONFIGURATION_GENERATOR_HEADER