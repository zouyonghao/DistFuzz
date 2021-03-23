#include <braft_test_common.hpp>

#include <dst_default_operator.hpp>
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
        key = (BRAFT_IS_START_KEY + node_name).c_str();
    }
    ~KillByName() {}
    bool _do() override
    {
        if (is_already_killed())
        {
            std::cout << node_name << " already killed!\n";
            return true;
        }
        dst_set_bool(key, false);
        return std::system(("ps -ef | grep " + name +
                            " | head -1 | awk '{print $2}' | xargs kill" +
                            (is_force_kill ? " -9" : ""))
                               .c_str()) == 0;
    }
};

REGISTER_OPERATOR(KillNode0, new KillByName("node0", "8300", false));
REGISTER_OPERATOR(KillNode1, new KillByName("node1", "8301", false));
REGISTER_OPERATOR(KillNode2, new KillByName("node2", "8303", false));

REGISTER_OPERATOR(ForceKillNode0, new KillByName("node0", "8300", true));
REGISTER_OPERATOR(ForceKillNode1, new KillByName("node1", "8301", true));
REGISTER_OPERATOR(ForceKillNode2, new KillByName("node2", "8303", true));