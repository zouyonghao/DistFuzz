// #ifndef CLICKHOUSE_COMMON_OPERATOR_HEADER
// #define CLICKHOUSE_COMMON_OPERATOR_HEADER

// #include <operator/dst_default_client_operator.hpp>

// class ClickHouseCommonOperator : public DefaultClientOperator
// {
// public:
//     ClickHouseCommonOperator(std::string _unfinished_command, std::vector<std::string> _need_random_parameters)
//         : DefaultClientOperator(_unfinished_command, "", _need_random_parameters)
//     {
//     }

//     virtual bool _do() override
//     {
//         // std::cout << unfinished_command << "\n";
//         std::string command = unfinished_command;
//         std::vector<std::string> op_vector;
//         for (auto &i : need_random_parameters)
//         {
//             uint32_t random_length = __dst_get_random_uint8_t();
//             std::string random(__dst_get_random_string(random_length));
//             std::cout << "get random string: ";
//             for (int i = 0; i < random.length(); i++)
//             {
//                 std::cout << std::hex << static_cast<int>(random[i]) << " ";
//             }
//             std::cout << "\n";
//             command += i + random + " ";
//             op_vector.push_back(random);
//         }
//         std::cout << command << "\n";

//         try
//         {
//             boost::process::ipstream pipe_stream;
//             boost::process::child c(command, boost::process::std_out > pipe_stream);
//             c.wait();
//             int result = c.exit_code();
//             // timeout, killed or force killed
//             if (result == 124 || result == 143 || result == 137)
//             {
//                 return result;
//             }
//             return true;
//         }
//         catch (const std::exception &e)
//         {
//             std::cerr << e.what() << '\n';
//             return false;
//         }
//     }
// };

// #endif // CLICKHOUSE_COMMON_OPERATOR_HEADER
