#ifndef DST_OPERATOR_H
#define DST_OPERATOR_H

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

#endif