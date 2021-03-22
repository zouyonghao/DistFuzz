#include <braft_test_common.hpp>

#include <dst_default_operator.hpp>
#include <dst_kv_store.h>

class StartByName : public SimpleCommandOperator
{
private:
    std::string node_name;
    const char *key;
    bool is_already_started()
    {
        // the node is started at first
        // and can be stopped by KillByName operators
        if (!exist(key))
        {
            return true;
        }
        return get_bool_or_not_exist(key);
    }

public:
    StartByName(std::string _node_name, std::string _command)
        : node_name(_node_name), SimpleCommandOperator(_command)
    {
        key = (BRAFT_IS_START_KEY + node_name).c_str();
    }
    ~StartByName() {}

    bool _do()
    {
        if (!is_already_started())
        {
            SimpleCommandOperator::_do();
            set_bool(key, true);
            return true;
        }
        std::cout << node_name << " already started\n";
        return true;
    }
};

REGISTER_OPERATOR(StartNode0, new StartByName("node0", "./start_node0.sh"));
REGISTER_OPERATOR(StartNode1, new StartByName("node1", "./start_node1.sh"));
REGISTER_OPERATOR(StartNode2, new StartByName("node2", "./start_node2.sh"));