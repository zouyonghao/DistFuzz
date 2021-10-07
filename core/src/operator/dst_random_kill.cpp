#include <dst_node_manager.hpp>
#include <dst_random.h>
#include <operator/dst_random_kill_operator.hpp>

bool RandomKill::_do()
{
    NodeManager *nm = SingletonRegistry<NodeManager>::getItem();
    std::vector<const NodeInfo *> alive_nodes;
    for (const NodeInfo &ni : nm->get_node_processes())
    {
        if (ni.should_alive)
        {
            alive_nodes.push_back(&ni);
        }
    }
    if (alive_nodes.empty())
    {
        std::cerr << "all nodes are stopped.\n";
        return true;
    }
    return nm->stop_node(alive_nodes[__dst_get_random_uint8_t() % nm->node_count]->node_id);
}

REGISTER_CRITICAL_OPERATOR(RandomKill, new RandomKill);