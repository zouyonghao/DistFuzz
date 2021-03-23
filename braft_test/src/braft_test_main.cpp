#include <iostream>
#include <thread>

#include <dst_kv_store.h>
#include <dst_operator.hpp>
#include <dst_operator_registry.hpp>
#include <dst_random.h>

using namespace std::chrono_literals;

int main(int argc, char const *argv[])
{
    uint32_t test_case_count = 0;
    while (true)
    {
        std::cout << "\033[1;31mrunning test case " << test_case_count
                  << "\033[0m\n";

        // prepare random files
        std::cout << "prepare random files\n";
        system("./generate_1m_bytes.sh random_node0.txt");
        system("./generate_1m_bytes.sh random_node1.txt");
        system("./generate_1m_bytes.sh random_node2.txt");
        system("./generate_1m_bytes.sh random_proxy.txt");
        // system("./generate_1m_bytes.sh random.txt");

        // start nodes and proxies
        std::cout << "start nodes and proxies\n";
        system("./run_fuzz_server.sh");

        std::this_thread::sleep_for(5s);
        // run 20 operations
        size_t operator_size =
            OperatorRegistry<Operator>::getItemVector().size();
        for (int i = 0; i < 20; i++)
        {
            std::this_thread::sleep_for(2s);
            uint32_t index = __dst_get_random_uint8_t() % operator_size;
            std::cout
                << "running operator "
                << OperatorRegistry<Operator>::getItemVector()[index].first
                << "\n";
            std::thread t1([index]() {
                OperatorRegistry<Operator>::getItemVector()[index]
                    .second->_do();
            });

            // run another operation
            index = __dst_get_random_uint8_t() % operator_size;
            std::cout
                << "running operator "
                << OperatorRegistry<Operator>::getItemVector()[index].first
                << "\n";
            std::thread t2([index]() {
                OperatorRegistry<Operator>::getItemVector()[index]
                    .second->_do();
            });
            t1.join();
            t2.join();
        }

        // stop
        std::cout << "stopping...\n";
        system("./stop.sh");
        dst_clear_all();

        // backup test cases
        std::cout << "backup test cases\n";
        system(("sh ./backup_test_case.sh " + std::to_string(test_case_count++))
                   .c_str());

        // std::this_thread::sleep_for(2s);
    }

    return 0;
}
