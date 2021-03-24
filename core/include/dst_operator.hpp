#ifndef DST_OPERATOR_H
#define DST_OPERATOR_H

#include <iostream>
#include <vector>
#include <dst_registry.hpp>

class Operator
{
private:
public:
    Operator() {}
    ~Operator() {}
    virtual bool _do() = 0;
};

#define REGISTER_OPERATOR(operator_name, operator)                             \
    bool operator_name##_entry =                                               \
        Registry<Operator>::add(#operator_name, operator)

#endif