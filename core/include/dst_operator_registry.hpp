#define REGISTER_OPERATOR(operator_name, operator)                             \
    bool operator_name##_entry =                                               \
        OperatorRegistry<Operator>::add(#operator_name, operator)

#include <functional>
#include <unordered_map>
#include <vector>

template <typename T> class OperatorRegistry
{
public:
    typedef std::unordered_map<std::string, T *> ItemMap;
    typedef std::vector<std::pair<std::string, T *>> ItemVector;

    static bool add(const std::string &name, T *target)
    {
        auto map = getItemMap();
        if (map.find(name) != map.end())
        {
            return false;
        }

        getItemMap()[name] = target;
        getItemVector().push_back(std::pair<std::string, T *>(name, target));
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

    static ItemVector &getItemVector()
    {
        static ItemVector vector;
        return vector;
    }
};