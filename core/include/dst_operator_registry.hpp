#define REGISTER_OPERATOR(operator_name, operator)                             \
    bool operator_name##_entry =                                               \
        OperatorRegistry<Operator>::add(#operator_name, operator)

#include <functional>
#include <unordered_map>

template <typename T> class OperatorRegistry
{
public:
    typedef std::unordered_map<std::string, T *> ItemMap;

    static bool add(const std::string &name, T *target)
    {
        auto map = getItemMap();
        if (map.find(name) != map.end())
        {
            return false;
        }

        getItemMap()[name] = target;
        return true;
    }

    static T *create(const std::string &name)
    {
        auto map = getItemMap();
        if (map.find(name) == map.end())
        {
            return nullptr;
        }

        return map[name];
    }

    // Use Meyer's singleton to prevent SIOF
    static ItemMap &getItemMap()
    {
        static ItemMap map;
        return map;
    }
};