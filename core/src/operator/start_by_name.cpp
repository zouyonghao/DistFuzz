#include <operator/dst_start_by_name_operator.hpp>

bool StartByName::_do()
{
    if (!is_already_started())
    {
        SimpleCommandOperator::_do();
        dst_set_bool(key, true);
        return true;
    }
    std::cout << node_name << " already started\n";
    return true;
}
