#ifndef DST_DEFAULT_CLIENT_OPERATOR_HEADER
#define DST_DEFAULT_CLIENT_OPERATOR_HEADER

#include <dst_event.h>
#include <dst_random.h>
#include <operator/dst_default_operator.hpp>
#include <util/common_uils.hpp>

#include <limits>
#include <map>
#include <sstream>

#include <boost/process.hpp>

class DefaultClientOperator : public NormalOperator
{
protected:
    std::string unfinished_command;
    std::string op_name;
    std::vector<std::string> need_random_parameters;

public:
    DefaultClientOperator(std::string _unfinished_command,
                          std::string _op_name,
                          std::vector<std::string> _need_random_parameters)
        : unfinished_command(_unfinished_command), op_name(_op_name),
          need_random_parameters(_need_random_parameters)
    {
    }

    virtual bool _do();
};

#endif // DST_DEFAULT_CLIENT_OPERATOR_HEADER
