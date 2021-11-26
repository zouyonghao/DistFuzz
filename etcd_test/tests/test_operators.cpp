#include <cassert>
#include <operator/dst_default_operator.hpp>

int main()
{
    /* RandomKill, RandomStart and NoopOperator */
    assert(Registry<CriticalOperator>::getItemMap().size() == 3);
    /* Init, EtcdSet, EtcdGet */
    assert(Registry<NormalOperator>::getItemMap().size() == 3);
    return 0;
}