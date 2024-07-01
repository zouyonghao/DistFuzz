#ifndef DST_START_BY_NAME_OPERATOR_HPP
#define DST_START_BY_NAME_OPERATOR_HPP

#include <dst_node_manager.hpp>
#include <dst_random.h>
#include <operator/dst_random_start_operator.hpp>

class StartNewNode : public CriticalOperator
{
public:
    StartNewNode() = default;
    bool _do() override
    {
        NodeManager *nm = SingletonRegistry<NodeManager>::getItem();
        nm->set_node_count(nm->get_node_count() + 1);
        
        uint32_t node_id = nm->get_node_count() - 1;
        NodeInfo ni;
        ni.node_id = node_id;
        ni.log_index = 0;
        ni.should_alive = false;
        ni.start_command = nm->configuration_generator->get_configure_string(node_id, nm->get_node_count());
        std::cout << "node " << node_id << " start command is : " << ni.start_command << "\n";
        nm->node_processes.push_back(ni);

        // need new fuzzing file, just make empty for now
        std::ofstream fuzzing_file("./random_node" + std::to_string(node_id));
        fuzzing_file.close();

        return nm->start_node(node_id);
    }
};

#endif
// REGISTER_CRITICAL_OPERATOR(StartNewNode, new StartNewNode);