#include <operator.hpp>
#include <operator_registry.hpp>

#include <iostream>

class RestartOperator : public Operator
{
public:
    RestartOperator() {}
    bool _do();
};

bool RestartOperator::_do()
{
    std::cout << "restart the target here!\n";
    return true;
}

REGISTER_OPERATOR(RestartOperator, new RestartOperator);