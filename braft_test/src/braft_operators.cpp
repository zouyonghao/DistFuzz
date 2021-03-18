#include <default_operator.hpp>
#include <dst_random.h>

class KillByName : public Operator
{
private:
    std::string name;

public:
    KillByName(std::string _name) : name(_name) {}
    ~KillByName() {}
    bool _do()
    {
        return std::system(("ps -ef | grep " + name +
                            " | head -1 | awk '{print $2}' | xargs kill -9")
                               .c_str()) == 0;
    }
};

REGISTER_OPERATOR(KillNode0, new KillByName("8300"));
REGISTER_OPERATOR(KillNode1, new KillByName("8301"));
REGISTER_OPERATOR(KillNode2, new KillByName("8303"));

class AtomicTestOperator : public Operator
{
private:
    std::string unfinished_command;
    std::vector<std::string> need_random_parameters;

public:
    AtomicTestOperator(std::string _conf, std::string _atomic_op,
                       std::vector<std::string> _need_random_parameters)
        : need_random_parameters(_need_random_parameters)
    {
        unfinished_command = "timeout 2 ./atomic_test -conf=" + _conf +
                             " -atomic_op=" + _atomic_op;
    }

    virtual bool _do()
    {
        std::string command = unfinished_command;
        for (auto &i : need_random_parameters)
        {
            command += " -" + i + "=" +
                       std::to_string(__dst_get_random_uint32()) + " ";
        }
        std::cout << command << "\n";
        return std::system(command.c_str()) == 0;
    }
};

// ./atomic_test -conf="127.0.1.1:8300:0,127.0.1.1:8301:0,127.0.1.1:8302:0,"
// -atomic_op="cas" -atomic_id=1 -atomic_val=10 -atomic_new_val=12
REGISTER_OPERATOR(AtomicTestGet,
                  new AtomicTestOperator(
                      "127.0.1.1:8300:0,127.0.1.1:8301:0,127.0.1.1:8302:0,",
                      "get", {"atomic_id"}));
REGISTER_OPERATOR(AtomicTestSet,
                  new AtomicTestOperator(
                      "127.0.1.1:8300:0,127.0.1.1:8301:0,127.0.1.1:8302:0,",
                      "set", {"atomic_id", "atomic_val"}));
REGISTER_OPERATOR(AtomicTestCas,
                  new AtomicTestOperator(
                      "127.0.1.1:8300:0,127.0.1.1:8301:0,127.0.1.1:8302:0,",
                      "get", {"atomic_id", "atomic_val", "atomic_new_val"}));