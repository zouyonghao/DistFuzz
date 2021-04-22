#ifndef DST_KILL_BY_NAME_OPERATOR_HPP
#define DST_KILL_BY_NAME_OPERATOR_HPP

#include <operator/dst_default_operator.hpp>
#include <dst_kv_store.h>

class KillByName : public Operator
{
private:
    std::string node_name;
    std::string name;
    bool is_force_kill;
    const char *key;

    bool is_already_killed()
    {
        if (dst_exist(key) && !dst_get_bool_or_not_exist(key))
        {
            return true;
        }
        return false;
    }

public:
    KillByName(std::string _node_name, std::string _name, bool _is_force_kill)
        : node_name(_node_name), name(_name), is_force_kill(_is_force_kill)
    {
        key = (IS_START_KEY + node_name).c_str();
    }
    ~KillByName() {}
    bool _do() override;
};

#endif