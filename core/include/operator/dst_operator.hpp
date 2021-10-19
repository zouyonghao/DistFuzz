#ifndef DST_OPERATOR_H
#define DST_OPERATOR_H

#include <dst_registry.hpp>
#include <iostream>
#include <vector>

class Operator
{
private:
public:
    Operator() {}
    ~Operator() {}
    virtual bool _do() = 0;
};

class CriticalOperator : public Operator
{
};

/* Usually this should be a client operator */
class NormalOperator : public Operator
{
public:
    uint32_t node_count;
};

#define INIT_OPERATOR_NAME "Init"

#define REGISTER_CRITICAL_OPERATOR(operator_name, operator)                                                            \
    bool operator_name##_entry = Registry<CriticalOperator>::add(#operator_name, operator)

#define REGISTER_NORMAL_OPERATOR(operator_name, operator)                                                              \
    bool operator_name##_entry = Registry<NormalOperator>::add(#operator_name, operator)

#endif