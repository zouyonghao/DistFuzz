#ifndef DST_DEFAULT_OPERATOR_HPP
#define DST_DEFAULT_OPERATOR_HPP

#include <operator/dst_operator.hpp>
#include <utility>

class SimpleCommandOperator : public NormalOperator
{
private:
    std::string command;

public:
    explicit SimpleCommandOperator(std::string _command) : command(std::move(_command)) {}
    bool _do() override;
    std::string &get_command() { return command; }
};

#endif