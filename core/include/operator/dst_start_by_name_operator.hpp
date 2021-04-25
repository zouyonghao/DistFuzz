#ifndef DST_START_BY_NAME_OPERATOR_HPP
#define DST_START_BY_NAME_OPERATOR_HPP

#include <dst_kv_store.h>
#include <operator/dst_default_operator.hpp>

class StartByName : public SimpleCommandOperator, public CriticalOperator
{
private:
    std::string node_name;
    const char *key;
    bool is_already_started()
    {
        // the node is started at first
        // and can be stopped by KillByName operators
        if (!dst_exist(key))
        {
            return true;
        }
        return dst_get_bool_or_not_exist(key);
    }

public:
    StartByName(std::string _node_name, std::string _command)
        : node_name(_node_name), SimpleCommandOperator(_command)
    {
        key = (IS_START_KEY + node_name).c_str();
    }
    ~StartByName() {}

    bool _do();
};

#endif