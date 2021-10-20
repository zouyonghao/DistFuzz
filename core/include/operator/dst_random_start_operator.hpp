#ifndef DST_START_BY_NAME_OPERATOR_HPP
#define DST_START_BY_NAME_OPERATOR_HPP

#include <dst_kv_store.h>
#include <operator/dst_default_operator.hpp>

class RandomStart : public CriticalOperator
{
public:
    RandomStart() = default;
    bool _do() override;
};

#endif