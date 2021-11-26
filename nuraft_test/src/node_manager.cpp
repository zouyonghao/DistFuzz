#include <nuraft_configuration_generator.hpp>

class NuRaftNodeManager : public NodeManager
{
public:
    NuRaftNodeManager(ServerConfigurationGenerator *scg) : NodeManager(scg) {}
    bool start_process(NodeInfo &ni) override
    {
        std::string node_id_str = std::to_string(ni.node_id);
        boost::process::environment env = boost::this_process::environment();
        // env["LD_PRELOAD"] = PRELOAD_LIB_PATH;
        env["__DST_ENV_RANDOM_FILE__"] = "random_node" + node_id_str;
        env["NODE_NAME"] = "node" + node_id_str;
        env["NODE_ID"] = node_id_str;
        env["DST_FUZZ"] = "1";
        std::string log_file = "log" + node_id_str + "_" + std::to_string(ni.log_index);
        std::string err_log_file = "log_err" + node_id_str + "_" + std::to_string(ni.log_index);
        if (ni.process != nullptr)
        {
            delete ni.process;
            ni.process = nullptr;
        }
        ni.process = new boost::process::child(
            ni.start_command, boost::process::std_out > log_file, boost::process::std_err > err_log_file, env,
            boost::process::std_in < "calc" + std::to_string(ni.node_id + 1) + ".pipe");
        boost::process::system("strace -f -p " + std::to_string(ni.process->id()), env);
        return true;
    }
};

REGISTER_NODE_MANAGER(NURAFT_NODE_MANAGER, new NuRaftNodeManager(new NuraftConfigurationGenerator()));
