//
// Created by zyh on 10/14/21.
//
#include <cassert>
#include <operator/dst_default_operator.hpp>

int main()
{
    /* RandomKill, RandomStart and NoopOperator */
    assert(Registry<CriticalOperator>::getItemMap().size() == 3);
    /* Init, RedisraftSet and RedisraftGet */
    assert(Registry<NormalOperator>::getItemMap().size() == 3);
    assert(Registry<NormalOperator>::getItemVector().size() == 3);

    for (auto o = Registry<NormalOperator>::getItemVector().begin();
         o != Registry<NormalOperator>::getItemVector().end(); o++)
    {
        if (o->first == INIT_OPERATOR_NAME)
        {
            std::cerr << "We have init operator!\n";
            // o->second->_do();
            /** remove the init operator after it is executed */
            Registry<NormalOperator>::getItemVector().erase(o);
            Registry<NormalOperator>::getItemMap().erase(INIT_OPERATOR_NAME);
            break;
        }
    }
    assert(Registry<NormalOperator>::getItemMap().size() == 2);
    assert(Registry<NormalOperator>::getItemVector().size() == 2);
    return 0;
}