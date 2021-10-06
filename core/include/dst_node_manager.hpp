#ifndef DST_NODE_MANAGER_HEADER
#define DST_NODE_MANAGER_HEADER

#include <boost/process.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <regex>

class NodeManager
{
public:
    std::string node_start_command;
    uint32_t node_count;

    NodeManager(std::string _node_start_command, uint32_t _node_count)
        : node_start_command(_node_start_command), node_count(_node_count)
    {
        
    }

    virtual bool start_all();

    /**
     * This function should check whether the process
     * which is not killed by some operators is still
     * alive.
     * Or something that is related to nodes.
     */
    virtual bool check();

    virtual bool stop_all();

    void wait_all();

private:
    std::vector<boost::process::child> node_processes;
};

bool NodeManager::start_all()
{
    for (uint32_t i = 0; i < node_count; i++)
    {
        std::string node_start_command_i = std::regex_replace(node_start_command, std::regex("\\$"), std::to_string (i));
        std::cout << "node " << i << " start command is : " << node_start_command_i << "\n";
        node_processes.push_back(boost::process::child(node_start_command_i, boost::process::std_out > "log" + std::to_string(i)));
    }
    return true;
}

bool NodeManager::check() { return true; }

bool NodeManager::stop_all() { return true; }

void NodeManager::wait_all()
{
    for (auto &c : node_processes)
    {
        c.wait();
    }
}

#endif // DST_NODE_MANAGER_HEADER