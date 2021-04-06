#include <dst_default_operator.hpp>
#include <dst_event.h>
#include <dst_random.h>

#include <limits>
#include <map>
#include <sstream>

#include <boost/process.hpp>

static const std::string ATOMIC_TEST_PATH = "/home/zyh/braft/example/atomic";

static std::map<std::string, std::string> OP_MAP = {
    {"get", "read"}, {"set", "write"}, {"cas", "cas"}};

class AtomicTestOperator : public Operator
{
private:
    std::string atomic_op;
    std::string unfinished_command;
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
                           ", :type :invoke, :f :" + OP_MAP[atomic_op] +
                           ", :value " + convert_value_vector(values) + "}");
    }

    std::string get_result_record(std::vector<std::string> &values, int result,
                                  std::string &last_output)
    {
        std::string value = convert_value_vector(values);
        if (atomic_op == "get")
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
                           ", :f :" + OP_MAP[atomic_op] + ", :value " + value +
                           "}");
    }

public:
    AtomicTestOperator(std::string _conf, std::string _atomic_op,
                       std::vector<std::string> _need_random_parameters)
        : atomic_op(_atomic_op), need_random_parameters(_need_random_parameters)
    {

        // TODO currently we only support one atomic_id (aka. one register)
        unfinished_command = "timeout 2 " + ATOMIC_TEST_PATH +
                             "/atomic_test -conf=" + _conf +
                             " -atomic_op=" + _atomic_op;
    }

    virtual bool _do()
    {
        std::string command = unfinished_command;
        std::vector<std::string> values;
        for (auto &i : need_random_parameters)
        {
            std::string random = std::to_string(__dst_get_random_uint32());
            command += " -" + i + "=" + random + " ";
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

        __dst_event_record(
            get_result_record(values, result, last_output).c_str());
        return result;
    }
};

// ./atomic_test -conf="127.0.1.1:8300:0,127.0.1.1:8301:0,127.0.1.1:8302:0,"
// -atomic_op="cas" -atomic_id=1 -atomic_val=10 -atomic_new_val=12
REGISTER_OPERATOR(AtomicTestGet,
                  new AtomicTestOperator(
                      "127.0.1.1:8300:0,127.0.1.1:8301:0,127.0.1.1:8302:0,",
                      "get", {}));
REGISTER_OPERATOR(AtomicTestSet,
                  new AtomicTestOperator(
                      "127.0.1.1:8300:0,127.0.1.1:8301:0,127.0.1.1:8302:0,",
                      "set", {"atomic_val"}));
REGISTER_OPERATOR(AtomicTestCas,
                  new AtomicTestOperator(
                      "127.0.1.1:8300:0,127.0.1.1:8301:0,127.0.1.1:8302:0,",
                      "cas", {"atomic_val", "atomic_new_val"}));