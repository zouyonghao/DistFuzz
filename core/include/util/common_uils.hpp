#ifndef __DST_COMMON_UTILS_H__
#define __DST_COMMON_UTILS_H__

#include <sstream>
#include <string>

#include <boost/process.hpp>

static std::string get_thread_id()
{
    std::stringstream ss;
    ss << std::this_thread::get_id();
    return std::to_string(std::stoull(ss.str()) % INT_MAX);
}

static std::string convert_value_vector(std::vector<std::string> &op_vector)
{
    switch (op_vector.size())
    {
    case 0:
        return "nil";
    case 1:
        return op_vector[0];
    default:
    {
        std::string result = "[";
        for (auto &t : op_vector)
        {
            result += t + " ";
        }
        result += "]";
        return result;
    }
    }
}

static std::string get_invoke_record(std::string op_name, std::vector<std::string> &op_vector, int thread_id)
{
    return std::string("{:process " + std::to_string(thread_id) + ", :type :invoke, :f :" + op_name + ", :value " +
                       convert_value_vector(op_vector) + "}");
}

static std::string get_result_record(std::string op_name, std::vector<std::string> &op_vector, int result,
                                     const std::string last_output, int thread_id)
{
    std::string value = convert_value_vector(op_vector);
    if (op_name == "read")
    {
        try
        {
            value = std::to_string(std::stoll(last_output));
        }
        catch (const std::exception &e)
        {
            // std::cerr << e.what() << '\n'; // stoi
        }
    }
    return std::string("{:process " + std::to_string(thread_id) + ", :type " + std::string(result == 0 ? ":ok" : ":fail") +
                       ", :f :" + op_name + ", :value " + value + "}");
}

#endif