#include <dst_node_manager.hpp>
#include <dst_random.h>
#include <operator/dst_random_start_operator.hpp>

bool RandomStart::_do()
{
    NodeManager *nm = SingletonRegistry<NodeManager>::getItem();
    std::vector<const NodeInfo *> stopped_nodes;
    for (const NodeInfo &ni : nm->get_node_processes())
    {
        if (!ni.should_alive)
        {
            stopped_nodes.push_back(&ni);
        }
    }
    if (stopped_nodes.empty())
    {
        std::cerr << "all nodes are alive.\n";
        return true;
    }
    return nm->start_node(stopped_nodes[__dst_get_random_uint8_t() % stopped_nodes.size()]->node_id);
}

REGISTER_CRITICAL_OPERATOR(RandomStart, new RandomStart);