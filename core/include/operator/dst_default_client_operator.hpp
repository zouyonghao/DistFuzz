#ifndef DST_DEFAULT_CLIENT_OPERATOR_HEADER
#define DST_DEFAULT_CLIENT_OPERATOR_HEADER

#include <dst_event.h>
#include <dst_random.h>
#include <operator/dst_default_operator.hpp>

#include <limits>
#include <map>
#include <sstream>

#include <boost/process.hpp>

class DefaultClientOperator : public Operator
{
private:
    std::string unfinished_command;
    std::string op_name;
    std::vector<std::string> need_random_parameters;

    std::string convert_value_vector(std::vector<std::string> &values)
    {
        switch (values.size())
        {
        case 0:
            return "nil";
        case 1:
            return values[0];
        default:
        {
            std::string result = "[";
            for (auto &t : values)
            {
                result += t + " ";
            }
            result += "]";
            return result;
        }
        }
    }

    std::string get_thread_id()
    {
        std::stringstream ss;
        ss << std::this_thread::get_id();
        return std::to_string(std::stoull(ss.str()) % INT_MAX);
    }

    std::string get_invoke_record(std::vector<std::string> &values)
    {
        return std::string("{:process " + get_thread_id() +
                           ", :type :invoke, :f :" + op_name + ", :value " +
                           convert_value_vector(values) + "}");
    }

    std::string get_result_record(std::vector<std::string> &values, int result,
                                  std::string &last_output)
    {
        std::string value = convert_value_vector(values);
        if (op_name == "get")
        {
            try
            {
                value = std::to_string(std::stoi(last_output));
            }
            catch (const std::exception &e)
            {
                std::cerr << e.what() << '\n';
            }
        }
        return std::string("{:process " + get_thread_id() + ", :type " +
                           std::string(result == 0 ? ":ok" : ":fail") +
                           ", :f :" + op_name + ", :value " + value + "}");
    }

public:
    DefaultClientOperator(std::string _unfinished_command, std::string _atomic_op,
                       std::vector<std::string> _need_random_parameters)
        : unfinished_command(_unfinished_command), op_name(_atomic_op),
          need_random_parameters(_need_random_parameters)
    {
    }

    virtual bool _do();
};

#endif // DST_DEFAULT_CLIENT_OPERATOR_HEADER
