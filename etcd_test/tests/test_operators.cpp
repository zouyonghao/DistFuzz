#include <cassert>
#include <operator/dst_default_operator.hpp>

int main()
{
    /* RandomKill, RandomStart and NoopOperator */
    assert(Registry<CriticalOperator>::getItemMap().size() == 3);
    /* KeeperSet, KeeperGet */
    assert(Registry<NormalOperator>::getItemMap().size() == 2);
    return 0;
}