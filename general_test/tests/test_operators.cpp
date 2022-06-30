#include <cassert>
#include <operator/dst_default_operator.hpp>

int main()
{
    /* RandomKill, RandomStart and NoopOperator */
    assert(Registry<CriticalOperator>::getItemMap().size() == 3);
    /* Init, DeleteDataFolder, KeeperSet, KeeperGet, KeeperSetWithVersion */
    assert(Registry<NormalOperator>::getItemMap().size() == 4);
    return 0;
}