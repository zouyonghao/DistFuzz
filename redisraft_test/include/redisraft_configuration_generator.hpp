#ifndef REDISRAFT_CONFIGURATION_GENERATOR_HEADER
#define REDISRAFT_CONFIGURATION_GENERATOR_HEADER

#include <cstdarg>
#include <operator/dst_default_client_operator.hpp>

#define BIN_PATH "/home/zyh/redis/src/redis-server"
#define LIB_PATH "/home/zyh/redisraft/redisraft.so"
#define BASE_PORT 5001
#define IP "127.0.1.1"

/* The ServerConfigurationGenerator for redisraft, the example is as follows:
 * redis-server --bind 127.0.1.1 --port 5001 --dbfilename raft1.rdb
 * --loadmodule $lib_path/redisraft.so raft-log-filename raftlog1.db addr 127.0.1.1:5001
 */
class RedisraftConfigurationGenerator : public ServerConfigurationGenerator
{
public:
    std::string get_configure_string(uint32_t node_id, uint32_t node_count) override
    {
        std::string config = BIN_PATH " --bind " IP " ";
        config += "--port " + std::to_string(BASE_PORT + node_id) + " ";
        config += "--dbfilename raft" + std::to_string(node_id) + ".rdb ";
        config += "--loadmodule " LIB_PATH " raft-log-filename raftlog" + std::to_string(node_id) + ".db ";
        config += "addr " IP ":" + std::to_string(BASE_PORT + node_id);
        return config;
    }
};

/* The ClientConfigurationGenerator for redisraft, the example is as follows:
 * timeout 2 bash /home/zyh/DistFuzz/redisraft_test/bin/run_client.sh get
 * timeout 2 bash /home/zyh/DistFuzz/redisraft_test/bin/run_client.sh set 1
 */
class RedisraftClientConfigurationGenerator : public ClientConfigurationGenerator
{
public:
    std::string get_configure_string(OP_NAME op_name, uint32_t node_count, ...)
    {
        std::string configure_string =
            "timeout 3 bash /home/zyh/DistFuzz/redisraft_test/bin/run_client.sh ";
        va_list random_nums;
        va_start(random_nums, node_count);
        switch (op_name)
        {
        case OP_READ:
            configure_string += "get";
            break;
        case OP_WRITE:
        {
            uint32_t value = va_arg(random_nums, uint32_t);
            configure_string += "set " + std::to_string(value);
            break;
        }
        case OP_CAS:
        {
            /* currently not supported */
            configure_string = "";
            break;
        }
        default:
            break;
        }
        va_end(random_nums);
        return configure_string;
    }
};

#endif // REDISRAFT_CONFIGURATION_GENERATOR_HEADER