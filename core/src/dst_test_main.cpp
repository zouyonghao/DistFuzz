#include <fstream>
#include <iostream>
#include <thread>
#include <vector>

#include <dst_kv_store.h>
#include <dst_node_manager.hpp>
#include <dst_random.h>
#include <log.hpp>
#include <operator/dst_operator.hpp>

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
            std::cout << "Run init operator\n";
            o->second->_do();
            /* remove the init operator after it is executed */
            Registry<NormalOperator>::getItemVector().erase(o);
            Registry<NormalOperator>::getItemMap().erase(INIT_OPERATOR_NAME);
            break;
        }
    }
}

void run_some_normal_operators(int number)
{
    size_t operator_size = Registry<NormalOperator>::getItemVector().size();
    std::cerr << "normal operator_size = " << operator_size << "\n";
    for (int i = 0; i < number && operator_size > 0; i++)
    {
        // std::this_thread::sleep_for(1s);
        uint32_t index = __dst_get_random_uint8_t() % operator_size;
        std::cout << "running operator " << Registry<NormalOperator>::getItemVector()[index].first << "\n";
        std::thread t1([index]() { Registry<NormalOperator>::getItemVector()[index].second->_do(); });

        // threads.push_back(std::thread([index]() { Registry<NormalOperator>::getItemVector()[index].second->_do();
        // }));

#ifndef NO_CONCURRENCY

        std::cout << "run concurrent normal operation\n";
        if (__dst_get_random_uint8_t() < 150)
        {
            // run another operation
            index = __dst_get_random_uint8_t() % operator_size;
            std::cout << "running operator " << Registry<NormalOperator>::getItemVector()[index].first << "\n";
            std::thread t2([index]() { Registry<NormalOperator>::getItemVector()[index].second->_do(); });

            // threads.push_back(
            //     std::thread([index]() { Registry<NormalOperator>::getItemVector()[index].second->_do(); }));
            t2.join();
        }

#endif // NO_CONCURRENCY
        t1.join();
    }
    // std::this_thread::sleep_for(std::chrono::seconds(3));
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
        ERROR_LOG("Open file " << initial_file << " failed.");
        exit(-1);
    }
    std::cout << "spliting files...\n";
    /* random.txt is used for test_main */
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
    std::cout << "backup test cases\n";
    system(("sh ./backup_test_case.sh " + std::to_string(test_case_count++)).c_str());
}

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage: xxx_test_main random_file [node_count]\n";
        exit(-1);
    }

    uint32_t node_count = 3;
    if (argc > 2)
    {
        node_count = std::atoi(argv[2]);
    }

    uint32_t test_case_count = 0;
    std::ifstream itest_case_count_file("test_case_count");
    itest_case_count_file >> test_case_count;
    itest_case_count_file.close();
    std::string random_file(argv[1]);
    split_files(random_file, node_count);

    size_t critical_operator_size = Registry<CriticalOperator>::getItemVector().size();

    std::cerr << "\033[1;31mrunning test case " << test_case_count << "\033[0m\n";
    std::cerr << "start nodes....\n";
    /* We should only have 1 NodeManager. */
    NodeManager *nm = SingletonRegistry<NodeManager>::getItem();
    if (nm == nullptr)
    {
        std::cerr << "failed to get NodeManager\n";
        goto STOP;
    }
    nm->set_node_count(node_count);
    if (!nm->start_all())
    {
        std::cout << "check failed!\n";
        goto STOP;
    }

    /* set node count for client operator */
    for (auto &i : Registry<NormalOperator>::getItemVector())
    {
        i.second->node_count = node_count;
    }

    std::this_thread::sleep_for(std::chrono::microseconds(__dst_get_random_uint16_t()));

    run_init_operator();
    std::cout << "normal operator run count = " << RUN_NORMAL_OPERATOR_COUNT << "\n";
    run_some_normal_operators(RUN_NORMAL_OPERATOR_COUNT);

    std::cout << "critical_operator_size = " << critical_operator_size << "\n";
    std::cout << "critical_operator run count = " << RUN_CRITICAL_OPERATOR_COUNT << "\n";
    for (int i = 0; i < RUN_CRITICAL_OPERATOR_COUNT && critical_operator_size > 0; i++)
    {
        std::this_thread::sleep_for(std::chrono::microseconds(__dst_get_random_uint16_t()));
        uint32_t index = __dst_get_random_uint8_t() % critical_operator_size;
        std::cout << "running operator " << Registry<CriticalOperator>::getItemVector()[index].first << "\n";
        Registry<CriticalOperator>::getItemVector()[index].second->_do();
        run_some_normal_operators(RUN_NORMAL_OPERATOR_COUNT);
    }

    // let it run a while
    std::this_thread::sleep_for(std::chrono::microseconds(__dst_get_random_uint16_t()));

    // check server availability
    if (!nm->check())
    {
        std::cout << "check failed!\n";
        goto STOP;
    }

STOP:
    std::cerr << "stopping...\n";
    nm->stop_all();

    run_some_normal_operators(RUN_NORMAL_OPERATOR_COUNT);

    /* we still need using kill to stop all process clearly */
    system("bash stop.sh");

    dst_clear_kv_all();

    backup_testcase(test_case_count);
    std::cout << test_case_count << "\n";

    std::ofstream otest_case_count_file("test_case_count");
    otest_case_count_file << test_case_count;
    otest_case_count_file.close();

    for (auto &t : threads)
    {
        t.join();
    }

    return 0;
}
