#include <operator/dst_default_client_operator.hpp>

std::string get_last_output(boost::process::ipstream &pipe_stream)
{
    /* the last output should be the result of read */
    std::string last_output;
    std::string tmp;
    while (pipe_stream && std::getline(pipe_stream, tmp) && !tmp.empty())
    {
        std::cerr << tmp << "\n";
        last_output = tmp;
    }
    return last_output;
}

bool DefaultClientOperator::_do()
{
    int random_thread_id = random() % INT_MAX;

    std::string command;
    uint32_t random_num1;
    uint32_t random_num2;
    std::string invoke_record_string =
        "{:process " + std::to_string(random_thread_id) + ", :type :invoke, :f :" + OP_NAME_STR[op_name] + ", :value ";
    switch (op_name)
    {
    case OP_READ:
        command = configuration_generator->get_configure_string(OP_READ, node_count);
        invoke_record_string += "nil";
        break;
    case OP_WRITE:
        random_num1 = __dst_get_random_uint32();
        command = configuration_generator->get_configure_string(OP_WRITE, node_count, random_num1);
        invoke_record_string += std::to_string(random_num1);
        break;
    case OP_CAS:
        random_num1 = __dst_get_random_uint32();
        random_num2 = __dst_get_random_uint32();
        command = configuration_generator->get_configure_string(OP_CAS, node_count, random_num1, random_num2);
        invoke_record_string += "[" + std::to_string(random_num1) + " " + std::to_string(random_num2) + "]";
        break;
    }
    if (command.empty())
    {
        std::cerr << OP_NAME_STR[op_name] << " is not supported!\n";
        return false;
    }
    invoke_record_string += "}";
    __dst_event_record(invoke_record_string.c_str());

    std::cerr << command << "\n";

    try
    {
        boost::process::ipstream pipe_stream;
        boost::process::child c(command, boost::process::std_out > pipe_stream);
        c.wait();
        int result = c.exit_code();
        /* timeout, killed or force killed */
        if (result == 124 || result == 143 || result == 137)
        {
            if (result == 124)
            {
                std::cerr << "client operator got timeout\n";
            }
            return false;
        }

        std::string result_record_string = "{:process " + std::to_string(random_thread_id) + ", :type " +
                                           std::string(result == 0 ? ":ok" : ":fail") +
                                           ", :f :" + OP_NAME_STR[op_name] + ", :value ";
        switch (op_name)
        {
        case OP_READ:
            result_record_string += std::to_string(std::stoll(get_last_output(pipe_stream)));
            break;
        case OP_WRITE:
            /* just print the std out to debug */
            get_last_output(pipe_stream);
            result_record_string += std::to_string(random_num1);
            break;
        case OP_CAS:
            /* just print the std out to debug */
            get_last_output(pipe_stream);
            result_record_string += "[" + std::to_string(random_num1) + " " + std::to_string(random_num2) + "]";
            break;
        }
        result_record_string += "}";
        __dst_event_record(result_record_string.c_str());
        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Fail to spawn a child process." << '\n';
        std::cerr << e.what() << '\n';
        return false;
    }
}