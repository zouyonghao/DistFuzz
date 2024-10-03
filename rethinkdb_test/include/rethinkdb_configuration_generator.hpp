#ifndef RETHINKDB_CONFIGURATION_GENERATOR_HEADER
#define RETHINKDB_CONFIGURATION_GENERATOR_HEADER

#include <cstdarg>
#include <operator/dst_default_client_operator.hpp>

#define BIN_PATH "/home/zyh/rethinkdb/build/release_clang_system/rethinkdb"
#define BASE_PORT 5000
#define BASE_DRIVER_PORT 4000
#define BASE_HTTP_PORT 8080
#define IP "127.0.1.1"

/* The ServerConfigurationGenerator for rethinkdb, the example is as follows:
 * rethinkdb --bind 127.0.1.1 --cluster-port 5000 --driver-port 4000 --http-port 8080
 * -d ./rethinkdb_data0 -j 127.0.1.1:5001 -j 127.0.1.1:5002
 */
class RethinkdbConfigurationGenerator : public ServerConfigurationGenerator
{
public:
    std::string get_configure_string(uint32_t node_id, uint32_t node_count)
    {
        std::string config = BIN_PATH " --bind " IP " ";
        config += "--cluster-port " + std::to_string(BASE_PORT + node_id) + " ";
        config += "--driver-port " + std::to_string(BASE_DRIVER_PORT + node_id) + " ";
        config += "--http-port " + std::to_string(BASE_HTTP_PORT + node_id) + " ";
        config += "-d ./rethinkdb_data" + std::to_string(node_id) + " ";
        for (int i = 0; i < node_count; i++)
        {
            if (i == node_id)
            {
                continue;
            }
            config += "-j " IP ":" + std::to_string(BASE_PORT + i) + " ";
        }
        return config;
    }
};

/* The ClientConfigurationGenerator for redisraft, the example is as follows:
 * /usr/bin/java -jar /home/zyh/DistFuzz/rethinkdb_test/client/target/
 * rethinkdb-test-client-1.0-SNAPSHOT-jar-with-dependencies.jar get
 * /usr/bin/java -jar /home/zyh/DistFuzz/rethinkdb_test/client/target/
 * rethinkdb-test-client-1.0-SNAPSHOT-jar-with-dependencies.jar set 1
 * /usr/bin/java -jar /home/zyh/DistFuzz/rethinkdb_test/client/target/
 * rethinkdb-test-client-1.0-SNAPSHOT-jar-with-dependencies.jar cas 1 2
 */
class RethinkdbClientConfigurationGenerator : public ClientConfigurationGenerator
{
public:
    std::string get_configure_string(OP_NAME op_name, uint32_t node_count, ...)
    {
        std::string configure_string = "timeout 3 /usr/bin/java -jar "
                                       "/home/zyh/DistFuzz/rethinkdb_test/client/target/"
                                       "rethinkdb-test-client-1.0-SNAPSHOT-jar-with-dependencies.jar " +
                                       std::to_string(node_count) + " ";
        va_list random_nums;
        va_start(random_nums, node_count);
        switch (op_name)
        {
        case OP_WRITE:
        {
            uint32_t value = va_arg(random_nums, uint32_t);
            configure_string += "set " + std::to_string(value);
            break;
        }
        case OP_READ:
            configure_string += "get";
            break;
        case OP_CAS:
        {
            uint32_t old_value = va_arg(random_nums, uint32_t);
            uint32_t new_value = va_arg(random_nums, uint32_t);
            configure_string += "cas " + std::to_string(old_value) + " " + std::to_string(new_value);
            break;
        }
        default:
            break;
        }
        va_end(random_nums);
        sleep(2);
        return configure_string;
    }
};

#endif // RETHINKDB_CONFIGURATION_GENERATOR_HEADER
