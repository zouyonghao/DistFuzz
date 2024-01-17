#ifndef DST_NODE_MANAGER_HEADER
#define DST_NODE_MANAGER_HEADER

#include <boost/process.hpp>
#include <csignal>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

#include <dst_configuration_generator.hpp>
#include <dst_log.hpp>
#include <dst_registry.hpp>

#define DEFAULT_NODE_COUNT 3

#define WAIT_PROCESS_START_MAX_COUNT 100000

#define WAIT_PROCESS_RR_STOP_COUNT 100000

struct NodeInfo
{
    /* The node ID */
    uint32_t node_id;
    /* should this node alive? */
    bool should_alive = true;
    /* update the log_index to redirect the output to
     * another log file when the node is restarted.
     */
    uint32_t log_index = 0;
    std::string start_command;
    boost::process::child *process = nullptr;
};

class NodeManager
{
public:
    uint32_t node_count;
    ServerConfigurationGenerator *configuration_generator;
    bool start_with_ebpf = false;
    bool start_with_rr = false;
    bool start_with_criu = false;

    explicit NodeManager(ServerConfigurationGenerator *_configuration_generator)
        : configuration_generator(_configuration_generator), node_count(DEFAULT_NODE_COUNT)
    {
    }

    void set_node_count(uint32_t _node_count) { node_count = _node_count; }

    uint32_t get_node_count() { return node_count; }

    virtual ~NodeManager()
    {
        for (auto &ni : node_processes)
        {
            if (ni.process != NULL)
            {
                delete ni.process;
                ni.process = nullptr;
            }
        }
    }

    virtual bool start_all()
    {
        for (uint32_t i = 0; i < node_count; i++)
        {
            NodeInfo ni;
            ni.node_id = i;
            ni.log_index = 0;
            ni.should_alive = true;
            ni.start_command = configuration_generator->get_configure_string(i, node_count);
            std::cout << "node " << i << " start command is : " << ni.start_command << "\n";
            node_processes.push_back(ni);
        }
        for (auto &ni : node_processes)
        {
            start_process(ni);
        }
        return check();
    }

    virtual bool stop_all()
    {
        for (NodeInfo &ni : node_processes)
        {
            if (start_with_rr)
            {
                kill(ni.process->id(), SIGTERM);
                ni.process->wait();
                // ni.process->wait_for(std::chrono::milliseconds(100));
                // if (ni.process->running())
                // {
                //     kill(ni.process->id(), SIGINT);
                //     ni.process->wait_for(std::chrono::milliseconds(100));
                // }
                // if (ni.process->running())
                // {
                //     kill(ni.process->id(), SIGKILL);
                //     LOG_ERROR << "node seems fail to stop, use SIGKILL to kill it.\n";
                //     ni.process->wait();
                // }
            }
            else
            {
                ni.process->terminate();
            }
            ni.should_alive = false;
        }
        return true;
    }

    virtual bool start_node(uint32_t i)
    {
        if (i > node_count - 1)
        {
            LOG_ERROR << "the index " << i << " too large, we only have " << node_count << " nodes.\n";
            return false;
        }
        NodeInfo &ni = node_processes[i];
        if (ni.should_alive)
        {
            LOG_ERROR << "the node " << i << " is alrealy started.\n";
            return true;
        }
        if (start_process(ni))
        {
            ni.should_alive = true;
            return true;
        }
        return false;
    }

    virtual bool stop_node(uint32_t i)
    {
        if (i > node_count - 1)
        {
            LOG_ERROR << "the index " << i << " too large, we only have " << node_count << " nodes.\n";
            return false;
        }
        NodeInfo &ni = node_processes[i];
        if (!ni.should_alive)
        {
            LOG_ERROR << "the node " << i << " is alrealy stoped.\n";
            return true;
        }

        if (start_with_rr)
        {
            kill(ni.process->id(), SIGTERM);
            ni.process->wait();
            // ni.process->wait_for(std::chrono::milliseconds(100));
            // if (ni.process->running())
            // {
            //     kill(ni.process->id(), SIGINT);
            //     ni.process->wait_for(std::chrono::milliseconds(100));
            // }
            // if (ni.process->running())
            // {
            //     kill(ni.process->id(), SIGKILL);
            //     LOG_ERROR << "node seems fail to stop, use SIGKILL to kill it.\n";
            //     ni.process->wait();
            // }
        }
        else
        {
            ni.process->terminate();
        }
        ni.should_alive = false;
        return true;
    }

    /**
     * This function should check whether the process
     * which is not killed by some operators is still
     * alive. Or something that is related to nodes.
     */
    virtual bool check()
    {
        for (NodeInfo &ni : node_processes)
        {
            if (ni.should_alive && !ni.process->running())
            {
                if (ni.process->exit_code() != 0)
                {
                    LOG_ERROR << "node " << ni.node_id << " is unintentially terminiated.\n";
                    return false;
                }
                else
                {
                    LOG_ERROR << "node " << ni.node_id << " is terminiated normally.\n";
                    /** TODO: true or false? */
                    return false;
                }
            }
            else if (!ni.should_alive && ni.process->running())
            {
                LOG_ERROR << "node " << ni.node_id << " should be killed but still running.\n";
                return false;
            }
        }
        return true;
    }

    const std::vector<NodeInfo> &get_node_processes() { return node_processes; }

    void wait_all()
    {
        for (auto &c : node_processes)
        {
            c.process->wait();
        }
    }

    virtual bool start_process(NodeInfo &ni)
    {
        std::string node_id_str = std::to_string(ni.node_id);
        boost::process::environment env = boost::this_process::environment();
        env["DATA_DIR"] = configuration_generator->get_data_folder(ni.node_id);
        // env["NO_FAULT_FILES"] = configuration_generator->get_no_fault_files(ni.node_id);
        env["__DST_ENV_RANDOM_FILE__"] = "random_node" + node_id_str;
        env["NODE_NAME"] = "node" + node_id_str;
        env["NODE_ID"] = node_id_str;
        env["DST_FUZZ"] = "1";
        // env["LD_PRELOAD"] = PRELOAD_LIB_PATH;
        std::string log_file = "log_app_" + node_id_str + "_" + std::to_string(ni.log_index);
        std::string err_log_file = "log_app_err" + node_id_str + "_" + std::to_string(ni.log_index);
        if (ni.process != nullptr)
        {
            delete ni.process;
            ni.process = nullptr;
        }
        if (start_with_ebpf)
        {
            ni.process = new boost::process::child(
                boost::process::search_path("dst_fuzz_fault").string() + " " + ni.start_command,
                boost::process::std_out > log_file, boost::process::std_err > err_log_file, env);
        }
        else if (start_with_rr)
        {
            ni.process = new boost::process::child(
                boost::process::search_path("rr").string() + " record -n -o rr_rec_" + node_id_str + "_" +
                    std::to_string(ni.log_index) + " " + ni.start_command,
                boost::process::std_out > log_file, boost::process::std_err > err_log_file, env);
        }
        else if (start_with_criu)
        {
            ni.process = new boost::process::child(
                boost::process::search_path("criu").string() + " dump -t " + ni.start_command, // TODO
                boost::process::std_out > log_file, boost::process::std_err > err_log_file, env);
        }
        else
        {
            ni.process = new boost::process::child(boost::process::search_path("strace").string() +
                                                       " -f -o strace_log_" + node_id_str + " " + ni.start_command,
                                                   boost::process::std_out > log_file,
                                                   boost::process::std_err > err_log_file, env);
        }
        int wait_count = 0;
        while (!ni.process->running() && (++wait_count) < WAIT_PROCESS_START_MAX_COUNT)
            ;
        ni.log_index++;
        return true;
    }

private:
    std::vector<NodeInfo> node_processes;
};

#define REGISTER_NODE_MANAGER(node_manager_name, node_manager)                                                         \
    bool node_manager_name##_entry = SingletonRegistry<NodeManager>::add(#node_manager_name, node_manager)

#endif // DST_NODE_MANAGER_HEADER