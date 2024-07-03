#include <dqlite_node_manager.hpp>
#include <dst_node_manager.hpp>
#include <dst_random.h>
#include <operator/dst_random_kill_operator.hpp>

class GraceRandomKill : public CriticalOperator
{
public:
    GraceRandomKill() = default;
    bool _do() override;
};

bool GraceRandomKill::_do()
{
    NodeManager *nm = SingletonRegistry<NodeManager>::getItem();
    DqliteNodeManager *dnm = dynamic_cast<DqliteNodeManager *>(nm);
    std::vector<const NodeInfo *> alive_nodes;
    for (const NodeInfo &ni : dnm->get_node_processes())
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
    return dnm->grace_stop_node(alive_nodes[__dst_get_random_uint8_t() % alive_nodes.size()]->node_id);
}

REGISTER_CRITICAL_OPERATOR(GraceRandomKill, new GraceRandomKill);