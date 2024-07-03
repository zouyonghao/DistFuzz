#include <cstdlib>
#include <fstream>
#include <iostream>
#include <regex>
#include <thread>
#include <vector>

#include <dst_kv_store.h>
#include <dst_log.hpp>
#include <dst_node_manager.hpp>
#include <dst_random.h>
#include <operator/dst_operator.hpp>
#include <utils/dst_color.h>
#include <utils/dst_share_mem_util.h>

#ifndef RUN_NORMAL_OPERATOR_COUNT
#define RUN_NORMAL_OPERATOR_COUNT 2
#endif

#ifndef RUN_CRITICAL_OPERATOR_COUNT
#define RUN_CRITICAL_OPERATOR_COUNT 2
#endif

std::vector<std::thread> threads;

void run_init_operator()
{
    for (auto o = Registry<NormalOperator>::getItemVector().begin();
         o != Registry<NormalOperator>::getItemVector().end(); o++)
    {
        if (o->first == INIT_OPERATOR_NAME)
        {
            LOG_INFO << "Run init operator\n";
            o->second->_do();
            /** remove the init operator after it is executed */
            Registry<NormalOperator>::getItemVector().erase(o);
            Registry<NormalOperator>::getItemMap().erase(INIT_OPERATOR_NAME);
            break;
        }
    }
}

void run_some_normal_operators(int number, int normal_sleep_ms, bool fuzz_normal_events, bool fuzz_time)
{
    size_t operator_size = Registry<NormalOperator>::getItemVector().size();
    for (int i = 0; i < number && operator_size > 0; i++)
    {
        std::this_thread::sleep_for(std::chrono::microseconds(normal_sleep_ms));
        uint32_t index = 0;
        if (fuzz_normal_events)
        {
            uint32_t index = __dst_get_random_uint8_t() % operator_size;
        }
        LOG_INFO << "running operator " << Registry<NormalOperator>::getItemVector()[index].first << "\n";
        std::thread t1([index]() { Registry<NormalOperator>::getItemVector()[index].second->_do(); });

        // threads.push_back(std::thread([index]() { Registry<NormalOperator>::getItemVector()[index].second->_do();
        // }));

#ifndef NO_CONCURRENCY

        std::cerr << "run concurrent normal operation\n";
        if (__dst_get_random_uint8_t() < 150)
        {
            // run another operation
            index = __dst_get_random_uint8_t() % operator_size;
            std::cerr << "running operator " << Registry<NormalOperator>::getItemVector()[index].first << "\n";
            std::thread t2([index]() { Registry<NormalOperator>::getItemVector()[index].second->_do(); });

            // threads.push_back(
            //     std::thread([index]() { Registry<NormalOperator>::getItemVector()[index].second->_do(); }));
            t2.join();
        }

#endif // NO_CONCURRENCY
        t1.join();
    }

    if (fuzz_time)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(__dst_get_random_uint8_t()));
    }
}

void split_files(std::string &initial_file, uint32_t node_count)
{
    /* split the file into several files */
    std::vector<char> content;
    std::ifstream file(initial_file, std::ios::binary);
    if (file)
    {
        file.seekg(0, std::ios::end);
        std::streampos length = file.tellg();
        file.seekg(0, std::ios::beg);
        content.resize(length);
        file.read(&content[0], length);
        file.close();
    }
    else
    {
        LOG_ERROR << "Open file " << initial_file << " failed.";
        exit(-1);
    }
    LOG_ERROR << "spliting files...\n";
    /** random.txt is used for test_main */
    std::vector<std::string> write_files{"random.txt"};
    for (uint32_t i = 0; i < node_count; i++)
    {
        write_files.push_back("random_node" + std::to_string(i));
    }

    uint64_t average_length = content.size() / (write_files.size());
    for (int f_index = 0; f_index < write_files.size(); f_index++)
    {
        std::vector<char> split_content;
        std::copy(content.begin() + average_length * f_index, content.begin() + average_length * (f_index + 1),
                  std::back_inserter(split_content));
        std::ofstream out(write_files[f_index], std::ios::out | std::ios::binary);
        out.write((const char *)&split_content[0], split_content.size());
        out.close();
    }
    __dst_reinit_random("random.txt");
}

void backup_testcase(uint32_t &test_case_count)
{
    LOG_ERROR << "backup test cases\n";
    system(("sh ./backup_test_case.sh " + std::to_string(test_case_count++)).c_str());
}

int main(int argc, char *argv[])
{
    std::map<std::string, std::string> options;
    std::regex optregex("--(help|fuzz_before_init|node_count|normal_sleep_ms|normal_count|critic_sleep_ms|critic_count|"
                        "no_fuzz_normal_events|no_fuzz_time|"
                        "check_after_fuzz|random_file|start_with_strace|start_with_ebpf|start_with_rr|use_checkpoint)"
                        "(?:=((?:.|\n)*))?");

    for (char **opt = argv + 1; opt < argv + argc; opt++)
    {
        std::smatch match;
        std::string s(*opt);
        if (std::regex_match(s, match, optregex))
        {
            options[std::string(match[1])] = match[2];
        }
        else
        {
            std::cerr << "Cannot parse option: " << *opt << std::endl;
            options["help"] = "";
        }
    }

    if (options.count("help"))
    {
        std::cerr << "    --help                  print this usage" << std::endl
                  << "    --node_count            the nodes needs to run" << std::endl
                  << "    --normal_sleep_ms       the sleep time between normal operators" << std::endl
                  << "    --normal_count          the normal operators count" << std::endl
                  << "    --critic_sleep_ms       the sleep time between critic operators" << std::endl
                  << "    --critic_count          the critic operators count" << std::endl
                  << "    --check_after_fuzz      whether to run some normal operators after fuzz" << std::endl
                  << "    --random_file           the random file name" << std::endl
                  << "    --fuzz_before_init      should we start fuzzing before the init operator (true or false)"
                  << std::endl
                  << "    --start_with_ebpf       start the process with dst_ebpf_fault" << std::endl
                  << "    --start_with_rr         start the process with rr" << std::endl
                  << std::endl
                  << "    --no_fuzz_normal_events with this option, normal events are generated randomly" << std::endl
                  << "    --no_fuzz_time          with this option, time interval is set to 100ms" << std::endl
                  << std::endl;
        return 0;
    }

    int node_count = DEFAULT_NODE_COUNT;
    if (options.count("node_count"))
    {
        node_count = std::atoi(options["node_count"].c_str());
    }

    bool check_after_fuzz = false;
    if (options.count("check_after_fuzz") && options["check_after_fuzz"] == "true")
    {
        check_after_fuzz = true;
    }

    std::string random_file = "init_random.txt";
    if (options.count("random_file"))
    {
        random_file = options["random_file"];
    }

    int run_normal_operator_count = RUN_NORMAL_OPERATOR_COUNT;
    if (options.count("normal_count"))
    {
        run_normal_operator_count = std::atoi(options["normal_count"].c_str());
    }

    int run_critic_operator_count = RUN_CRITICAL_OPERATOR_COUNT;
    if (options.count("critic_count"))
    {
        run_critic_operator_count = std::atoi(options["critic_count"].c_str());
    }

    int normal_sleep_ms = 0;
    if (options.count("normal_sleep_ms"))
    {
        normal_sleep_ms = std::atoi(options["normal_sleep_ms"].c_str());
    }

    int critic_sleep_ms = 0;
    if (options.count("critic_sleep_ms"))
    {
        critic_sleep_ms = std::atoi(options["critic_sleep_ms"].c_str());
    }

    bool fuzz_before_init = true;
    if (options.count("fuzz_before_init") && options["fuzz_before_init"] == "false")
    {
        fuzz_before_init = false;
    }

    bool start_with_ebpf = false;
    if (options.count("start_with_ebpf"))
    {
        start_with_ebpf = true;
    }

    bool start_with_rr = false;
    if (options.count("start_with_rr"))
    {
        start_with_rr = true;
    }

    bool use_checkpoint = false;
    if (options.count("use_checkpoint"))
    {
        use_checkpoint = true;
    }

    bool fuzz_normal_events = true;
    if (options.count("no_fuzz_normal_events"))
    {
        fuzz_normal_events = false;
    }

    bool fuzz_time = true;
    if (options.count("no_fuzz_time"))
    {
        fuzz_time = false;
    }

    log_init("log_test");

    uint32_t test_case_count = 0;
    std::ifstream itest_case_count_file("test_case_count");
    itest_case_count_file >> test_case_count;
    itest_case_count_file.close();

    system("sh env_init.sh");

    split_files(random_file, node_count);

    size_t critical_operator_size = Registry<CriticalOperator>::getItemVector().size();
    size_t normal_operator_size = Registry<NormalOperator>::getItemVector().size();
    LOG_INFO << "normal_operator_size = " << normal_operator_size << "\n";
    LOG_INFO << "normal operator run count = " << run_normal_operator_count << "\n";
    LOG_INFO << "critical_operator_size = " << critical_operator_size << "\n";
    LOG_INFO << "critical_operator run count = " << run_critic_operator_count << "\n";

    init_is_fuzzing();
    if (!fuzz_before_init)
    {
        /** Do not set is_fuzzing to false for some cases during boot */
        set_is_fuzzing(false);
    }

    bool check_failed = false;
    bool all_operators_after_fuzzing_failed = true;

    LOG_INFO << "\033[1;31m" << "running test case " << test_case_count << "\033[0m\n";
    LOG_INFO << "start nodes....\n";
    /** We should only have 1 NodeManager. */
    NodeManager *nm = SingletonRegistry<NodeManager>::getItem();
    if (nm == nullptr)
    {
        LOG_ERROR << "failed to get NodeManager\n";
        goto STOP;
    }
    nm->set_node_count(node_count);

    if (start_with_ebpf)
    {
        nm->start_with_ebpf = true;
    }

    if (start_with_rr)
    {
        nm->start_with_rr = true;
    }

    if (use_checkpoint)
    {
        nm->start_with_criu = true;
    }

    if (!nm->start_all())
    {
        check_failed = true;
        LOG_ERROR << "check failed!\n";
        goto STOP;
    }

    /** set node count for client operator */
    for (auto &i : Registry<NormalOperator>::getItemVector())
    {
        i.second->node_count = node_count;
    }

    if (fuzz_time)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(__dst_get_random_uint8_t()));
    }

    run_init_operator();
    set_is_fuzzing(true);
    /** the normal operator may change after running init operator */
    normal_operator_size = Registry<NormalOperator>::getItemVector().size();
    run_some_normal_operators(run_normal_operator_count, normal_sleep_ms, fuzz_normal_events, fuzz_time);

    for (int i = 0; i < run_critic_operator_count && critical_operator_size > 0; i++)
    {
        if (fuzz_time)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(__dst_get_random_uint8_t() + critic_sleep_ms));
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(critic_sleep_ms));
        }
        uint32_t index = __dst_get_random_uint8_t() % critical_operator_size;
        LOG_INFO << "running operator " << Registry<CriticalOperator>::getItemVector()[index].first << "\n";
        Registry<CriticalOperator>::getItemVector()[index].second->_do();
        run_some_normal_operators(run_normal_operator_count, normal_sleep_ms, fuzz_normal_events, fuzz_time);
    }

    /** check server availability */
    if (!nm->check())
    {
        check_failed = true;
        LOG_ERROR << "check failed!\n";
        goto STOP;
    }

    /** recovery checker to check whether servers can recover after fuzzing */
    if (check_after_fuzz)
    {
        int alive_node_count = 0;
        for (auto &ni : nm->get_node_processes())
        {
            if (ni.should_alive)
            {
                alive_node_count++;
            }
            else
            {
                LOG_ERROR << "node " << ni.node_id << " is stopped.\n";
            }
        }
        LOG_ERROR << "The remaining alive node count is " << alive_node_count << "\n";
        set_is_fuzzing(false);
        LOG_INFO << "Fuzzing is stopped, now we wait for 3 seconds and then"
                    " run some normal operators to see whether it works as usual.\n";
        std::this_thread::sleep_for(std::chrono::seconds(3));

        for (int i = 0; i < run_normal_operator_count && normal_operator_size > 0; i++)
        {
            uint32_t index = __dst_get_random_uint8_t() % normal_operator_size;
            LOG_INFO << "running operator " << Registry<NormalOperator>::getItemVector()[index].first << "\n";
            auto &running_operator = Registry<NormalOperator>::getItemVector()[index];
            if (running_operator.second->_do())
            {
                all_operators_after_fuzzing_failed = false;
            }
            else
            {
                LOG_ERROR << "running operator " << running_operator.first << " failed after fuzzing\n";
            }
        }
        if (all_operators_after_fuzzing_failed)
        {
            check_failed = true;
            LOG_ERROR << COLOR_RED_START << "all normal operators after fuzzing failed! :(" << COLOR_RESET_END << "\n";
        }
    }

    set_is_fuzzing(false);
    /** run some checkers before stop */
    system("sh check_before_stop.sh");

STOP:
    LOG_ERROR << "stopping...\n";
    nm->stop_all();

    remove_is_fuzzing();

    // run_some_normal_operators(RUN_NORMAL_OPERATOR_COUNT);

    /* we still need using kill to stop all process clearly */
    system("sh stop.sh");

    dst_clear_kv_all();

    if (check_failed)
    {
        LOG_ERROR << "check failed!";
    }

    backup_testcase(test_case_count);
    LOG_INFO << test_case_count << "\n";

    std::ofstream otest_case_count_file("test_case_count");
    otest_case_count_file << test_case_count;
    otest_case_count_file.close();

    for (auto &t : threads)
    {
        t.join();
    }

    if (check_failed)
    {
        abort();
    }

    return 0;
}
