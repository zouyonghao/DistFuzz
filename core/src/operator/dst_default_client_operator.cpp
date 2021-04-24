#include <operator/dst_default_client_operator.hpp>

bool DefaultClientOperator::_do()
{
    std::string command = unfinished_command;
    std::vector<std::string> values;
    for (auto &i : need_random_parameters)
    {
        std::string random = std::to_string(__dst_get_random_uint32());
        command += i + random + " ";
        values.push_back(random);
    }
    __dst_event_record(get_invoke_record(values).c_str());
    std::cout << command << "\n";
    // int result = std::system(command.c_str());

    boost::process::ipstream pipe_stream;
    boost::process::child c(command, boost::process::std_out > pipe_stream);
    c.wait();
    int result = c.exit_code();

    std::string tmp;
    std::string last_output;
    while (pipe_stream && std::getline(pipe_stream, tmp) && !tmp.empty())
    {
        last_output = tmp;
    }

    __dst_event_record(get_result_record(values, result, last_output).c_str());
    return result;
}