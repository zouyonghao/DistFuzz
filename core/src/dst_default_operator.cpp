
#include <dst_default_operator.hpp>

#include <iostream>

bool SimpleCommandOperator::_do()
{
    std::cout << "executing command `" << get_command() << "`\n";
    return system(get_command().c_str()) == 0;
}

REGISTER_OPERATOR(NoopOperator, new SimpleCommandOperator("Echo do nothing"));