#include <operator/dst_default_client_operator.hpp>

bool DefaultClientOperator::_do()
{
    // std::cout << unfinished_command << "\n";
    std::string command = unfinished_command;
    std::vector<std::string> op_vector;
    for (auto &i : need_random_parameters)
    {
        std::string random = std::to_string(__dst_get_random_uint32());
        command += i + random + " ";
        op_vector.push_back(random);
    }
    __dst_event_record(get_invoke_record(op_name, op_vector).c_str());
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

        __dst_event_record(get_result_record(op_name, op_vector, result, last_output).c_str());
        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }
}