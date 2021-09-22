#include <operator/dst_default_client_operator.hpp>

bool DefaultClientOperator::_do()
{
    int random_thread_id = random() % INT_MAX;
    // std::cout << unfinished_command << "\n";
    std::string command = unfinished_command;
    std::vector<std::string> op_vector;
    for (auto &i : need_random_parameters)
    {
        std::string random = std::to_string(__dst_get_random_uint32());
        command += i + random + " ";
        op_vector.push_back(random);
    }
    if (!op_name.empty()) // TODO: create another simple operator that just call a command.
        __dst_event_record(get_invoke_record(op_name, op_vector, random_thread_id).c_str());
    std::cerr << command << "\n";
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
            // std::cout << tmp << "\n";
            last_output = tmp;
        }
        if (!op_name.empty())
            __dst_event_record(get_result_record(op_name, op_vector, result, last_output, random_thread_id).c_str());
        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Fail to spawn a child process." << '\n';
        std::cerr << e.what() << '\n';
        return false;
    }
}