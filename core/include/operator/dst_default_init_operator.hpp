#include <operator/dst_default_client_operator.hpp>

/** The InitOperator will try to set until the read is success */
class DefaultInitOperator : public NormalOperator
{
public:
    int MAX_TRY_COUNT = 5;
    DefaultInitOperator() : NormalOperator() {}
    DefaultInitOperator(int max_try_count) : NormalOperator(), MAX_TRY_COUNT(max_try_count) {}
    virtual std::string get_write_zero_command() = 0;

    bool _do() override
    {
        int count = 0;
        while (count < MAX_TRY_COUNT)
        {
            usleep(500 * 1e3); // 500ms
            boost::process::child c(get_write_zero_command());
            c.wait();
            if (c.exit_code() == 0)
            {
                break;
            }
            count++;
        }
        if (count >= MAX_TRY_COUNT)
        {
            std::cerr << "Run init failed!\n";
            return false;
        }

        int random_thread_id = random() % INT_MAX;
        std::string invoke_record_string =
            "{:process " + std::to_string(random_thread_id) + ", :type :invoke, :f :write, :value 0}";
        __dst_event_record(invoke_record_string.c_str());
        std::string result_record_string =
            "{:process " + std::to_string(random_thread_id) + ", :type :ok, :f :write, :value 0}";
        __dst_event_record(result_record_string.c_str());
        return true;
    }
};