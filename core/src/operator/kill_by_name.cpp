#include <operator/dst_kill_by_name_operator.hpp>

bool KillByName::_do()
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
