//
// Created by zyh on 10/14/21.
//
#include <cassert>
#include <operator/dst_default_operator.hpp>

int main()
{
    /* RandomKill, RandomStart and NoopOperator */
    assert(Registry<CriticalOperator>::getItemMap().size() == 3);
    /* RethinkdbGet, RethinkdbSet and RethinkdbCas */
    assert(Registry<NormalOperator>::getItemMap().size() == 3);
    return 0;
}