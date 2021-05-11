#include <operator/dst_default_client_operator.hpp>

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

std::string get_invoke_record(std::string op_name,
                              std::vector<std::string> &values)
{
    return std::string("{:process " + get_thread_id() +
                       ", :type :invoke, :f :" + op_name + ", :value " +
                       convert_value_vector(values) + "}");
}

std::string get_result_record(std::string op_name,
                              std::vector<std::string> &values, int result,
                              std::string &last_output)
{
    std::string value = convert_value_vector(values);
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
    return std::string("{:process " + get_thread_id() + ", :type " +
                       std::string(result == 0 ? ":ok" : ":fail") +
                       ", :f :" + op_name + ", :value " + value + "}");
}

bool DefaultClientOperator::_do()
{
    // std::cout << unfinished_command << "\n";
    std::string command = unfinished_command;
    std::vector<std::string> values;
    for (auto &i : need_random_parameters)
    {
        std::string random = std::to_string(__dst_get_random_uint32());
        command += i + random + " ";
        values.push_back(random);
    }
    __dst_event_record(get_invoke_record(op_name, values).c_str());
    std::cout << command << "\n";
    // int result = std::system(command.c_str());

    try
    {
        boost::process::ipstream pipe_stream;
        boost::process::child c(command, boost::process::std_out > pipe_stream);
        c.wait();
        int result = c.exit_code();
        // timeout, killed or force killed
        if (result == 124 || result == 143 || result == 137)
        {
            return result;
        }

        std::string tmp;
        std::string last_output;
        while (pipe_stream && std::getline(pipe_stream, tmp) && !tmp.empty())
        {
            last_output = tmp;
        }

        __dst_event_record(
            get_result_record(op_name, values, result, last_output).c_str());
        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }
}