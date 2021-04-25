
#include <operator/dst_default_operator.hpp>

#include <iostream>

bool SimpleCommandOperator::_do()
{
    std::cout << "executing command `" << get_command() << "`\n";
    return system(get_command().c_str()) == 0;
}

class NoopOperator : public SimpleCommandOperator, public CriticalOperator
{
public:
    NoopOperator() : SimpleCommandOperator("echo do nothing") {}
    bool _do() { return ::SimpleCommandOperator::_do(); }
};

REGISTER_CRITICAL_OPERATOR(NoopOperator, new NoopOperator);