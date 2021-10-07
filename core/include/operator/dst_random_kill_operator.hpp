#ifndef DST_KILL_BY_NAME_OPERATOR_HPP
#define DST_KILL_BY_NAME_OPERATOR_HPP

#include <dst_kv_store.h>
#include <operator/dst_default_operator.hpp>

class RandomKill : public CriticalOperator
{
public:
    RandomKill() {}
    bool _do() override;
};

#endif