
#include <default_operator.hpp>

#include <iostream>

bool SimpleCommandOperator::_do()
{
    std::cout << "executing command `" << get_command() << "`\n";
    return system(get_command().c_str()) == 0;
}