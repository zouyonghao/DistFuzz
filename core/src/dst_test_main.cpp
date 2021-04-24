#include <fstream>
#include <iostream>
#include <thread>
#include <vector>

#include <dst_kv_store.h>
#include <dst_random.h>
#include <log.hpp>
#include <operator/dst_operator.hpp>

using namespace std::chrono_literals;

int main(int argc, char const *argv[])
{
    bool is_random_test = true;
    if (argc > 1)
    {
        is_random_test = false;
    }

    uint32_t test_case_count = 0;
    std::ifstream itest_case_count_file("test_case_count");
    itest_case_count_file >> test_case_count;
    itest_case_count_file.close();
    if (is_random_test)
    {
        // prepare random files
        std::cout << "prepare random files\n";
        system("./generate_1m_bytes.sh random_node0.txt");
        system("./generate_1m_bytes.sh random_node1.txt");
        system("./generate_1m_bytes.sh random_node2.txt");
        system("./generate_1m_bytes.sh random_proxy.txt");
        // system("./generate_1m_bytes.sh random.txt");
    }
    else
    {
        // split the file into several files
        std::vector<char> content;
        std::ifstream file(argv[1], std::ios::binary);
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
            ERROR_LOG("Open file " << argv[1] << " failed.");
            exit(-1);
        }
        std::cout << "spliting files...\n";
        std::vector<std::string> write_files{
            "random_node0.txt", "random_node1.txt", "random_node2.txt",
            "random_proxy.txt", "random.txt"};
        uint64_t average_length = content.size() / (write_files.size());
        for (int f_index = 0; f_index < write_files.size(); f_index++)
        {
            std::vector<char> split_content;
            std::copy(content.begin() + average_length * f_index,
                      content.begin() + average_length * (f_index + 1),
                      std::back_inserter(split_content));
            std::ofstream out(write_files[f_index],
                              std::ios::out | std::ios::binary);
            out.write((const char *)&split_content[0], split_content.size());
            out.close();
        }
        __dst_reinit_random(argv[1]);
    }

    std::cout << "\033[1;31mrunning test case " << test_case_count
              << "\033[0m\n";

    // start nodes and proxies
    std::cout << "start nodes and proxies\n";
    system("./run_fuzz_server.sh");

    std::this_thread::sleep_for(5s);

    // run 20 operations
    size_t operator_size = Registry<Operator>::getItemVector().size();
    std::cout << "operator_size = " << operator_size << "\n";
    for (int i = 0; i < 20; i++)
    {
        std::this_thread::sleep_for(2s);
        uint32_t index = __dst_get_random_uint8_t() % operator_size;
        std::cout << "running operator "
                  << Registry<Operator>::getItemVector()[index].first << "\n";
        std::thread t1([index]() {
            Registry<Operator>::getItemVector()[index].second->_do();
        });

        if (__dst_get_random_uint8_t() < 150)
        {
            // run another operation
            index = __dst_get_random_uint8_t() % operator_size;
            std::cout << "running operator "
                      << Registry<Operator>::getItemVector()[index].first
                      << "\n";
            std::thread t2([index]() {
                Registry<Operator>::getItemVector()[index].second->_do();
            });
            t2.join();
        }

        t1.join();
    }

    // stop
    std::cout << "stopping...\n";
    system("./stop.sh");
    dst_clear_kv_all();

    // backup test cases
    std::cout << "backup test cases\n";
    system(("sh ./backup_test_case.sh " + std::to_string(test_case_count++))
               .c_str());
    std::cout << test_case_count << "\n";

    std::ofstream otest_case_count_file("test_case_count");
    otest_case_count_file << test_case_count;
    otest_case_count_file.close();

    // std::this_thread::sleep_for(2s);

    return 0;
}
