#include <fstream>
#include <iostream>
#include <thread>
#include <vector>

#include <dst_kv_store.h>
#include <dst_random.h>
#include <log.hpp>
#include <operator/dst_operator.hpp>

std::vector<std::thread> threads;

void run_init_operator()
{
    for (auto &o : Registry<NormalOperator>::getItemVector())
    {
        if (o.first == INIT_OPERATOR_NAME)
        {
            std::cout << "Run init operator\n";
            o.second->_do();
        }
    }
}

void run_some_normal_operators(int number)
{
    size_t operator_size = Registry<NormalOperator>::getItemVector().size();
    std::cout << "normal operator_size = " << operator_size << "\n";
    for (int i = 0; i < number && operator_size > 0; i++)
    {
        // std::this_thread::sleep_for(1s);
        uint32_t index = __dst_get_random_uint8_t() % operator_size;
        std::cout << "running operator " << Registry<NormalOperator>::getItemVector()[index].first << "\n";
        // std::thread t1([index]() {
        //     Registry<NormalOperator>::getItemVector()[index].second->_do();
        // });

        threads.push_back(std::thread([index]() { Registry<NormalOperator>::getItemVector()[index].second->_do(); }));

#ifndef NO_CONCURRENCY

        std::cout << "run concurrent normal operation\n";
        if (__dst_get_random_uint8_t() < 150)
        {
            // run another operation
            index = __dst_get_random_uint8_t() % operator_size;
            std::cout << "running operator " << Registry<NormalOperator>::getItemVector()[index].first << "\n";
            // std::thread t2([index]() {
            //     Registry<NormalOperator>::getItemVector()[index].second->_do();
            // });

            threads.push_back(
                std::thread([index]() { Registry<NormalOperator>::getItemVector()[index].second->_do(); }));
            // t2.join();
        }

#endif // NO_CONCURRENCY
        //    t1.join();
    }
    std::this_thread::sleep_for(std::chrono::seconds(3));
}

void split_files(std::string initial_file)
{
    // split the file into several files
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
    std::vector<std::string> write_files{"random_node0.txt", "random_node1.txt", "random_node2.txt", "random_proxy.txt",
                                         "random.txt"};
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

int main(int argc, char const *argv[])
{
    if (argc < 1)
    {
        std::cout << "Usage: xxx_test_main random_file\n";
        exit(-1);
    }

    uint32_t test_case_count = 0;
    std::ifstream itest_case_count_file("test_case_count");
    itest_case_count_file >> test_case_count;
    itest_case_count_file.close();
    split_files(argv[1]);

    std::cout << "\033[1;31mrunning test case " << test_case_count << "\033[0m\n";

    std::cout << "start nodes and proxies\n";
    system("./run_fuzz_server.sh");

    // std::this_thread::sleep_for(std::chrono::microseconds(__dst_get_random_uint16_t()));

    run_init_operator();

    run_some_normal_operators(5);

    size_t operator_size = Registry<CriticalOperator>::getItemVector().size();
    std::cout << "operator_size = " << operator_size << "\n";
    for (int i = 0; i < 5 && operator_size > 0; i++)
    {
        std::this_thread::sleep_for(std::chrono::microseconds(__dst_get_random_uint16_t()));
        uint32_t index = __dst_get_random_uint8_t() % operator_size;
        std::cout << "running operator " << Registry<CriticalOperator>::getItemVector()[index].first << "\n";
        std::thread t1([index]() { Registry<CriticalOperator>::getItemVector()[index].second->_do(); });
        run_some_normal_operators(5);
        t1.join();
    }

    // let it run a while
    std::this_thread::sleep_for(std::chrono::microseconds(__dst_get_random_uint16_t()));

    std::cout << "stopping...\n";
    system("./stop.sh");

    run_some_normal_operators(5);

    dst_clear_kv_all();

    std::cout << "backup test cases\n";
    system(("sh ./backup_test_case.sh " + std::to_string(test_case_count++)).c_str());
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
