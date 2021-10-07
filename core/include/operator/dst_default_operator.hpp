#ifndef DST_DEFAULT_OPERATOR_HPP
#define DST_DEFAULT_OPERATOR_HPP

#include <operator/dst_operator.hpp>

class SimpleCommandOperator : public Operator
{
private:
    std::string command;

public:
    SimpleCommandOperator(std::string _command) : command(_command) {}
    bool _do();
    std::string &get_command() { return command; }
};

#endif