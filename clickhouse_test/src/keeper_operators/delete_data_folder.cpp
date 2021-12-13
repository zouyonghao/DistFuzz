#include <dst_random.h>
#include <operator/dst_operator.hpp>

class RandomDeleteFolderOperator : public NormalOperator
{
public:
    bool _do()
    {
        int result = system(("rm -rf run/log" + std::to_string(__dst_get_random_uint8_t() % node_count)).c_str());
        return result == 0;
    }
};

// REGISTER_NORMAL_OPERATOR(RandomDeleteFolderOperator, new RandomDeleteFolderOperator);
