#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

void a() { std::cout << std::stoll("3876838356") << "\n"; }

int main(int argc, char const *argv[])
{
    unsigned char tmp = 0;

    unsigned int result = tmp;
    result <<= 8u;

    tmp = 1;
    result |= tmp;
    result <<= 8u;

    tmp = 0;
    result |= tmp;
    result <<= 8u;

    tmp = 0;
    result |= tmp;

    printf("result = %d\n", result);

    std::vector<const char *> test{"echo", "abc"};
    if (fork() == 0)
    {
        execvp(test[0], (char *const *)&test[0]);
    }

    std::string test_str("/usr/share/cmake-3.10/Modules/CMakeCCompilerABI.c");
    test_str = test_str.substr(test_str.rfind("/") + 1,
                               test_str.rfind(".") - test_str.rfind("/") - 1);
    std::cout << test_str << "\n";

    test_str = "CMakeCCompilerABI.c";
    test_str = test_str.substr(test_str.rfind("/") + 1,
                               test_str.rfind(".") - test_str.rfind("/") - 1);
    std::cout << test_str << "\n";

    printf("result = %d\n", result);

    uint32_t test_case_count = 1;
    std::ifstream test_case_count_file("test_case_count");
    if (!test_case_count_file.is_open())
    {
        std::cout << "failed to open "
                  << "test_case_count" << '\n';
    }
    test_case_count_file >> test_case_count;
    std::cout << test_case_count << "\n";
    // test_case_count++;

    // test_case_count_file << test_case_count;
    // test_case_count_file.close();
    try
    {
        std::cout << std::stoi("abacab") << "\n";
    }
    catch (const std::exception &e)
    {
        std::cout << "exception!!" << "\n";
        std::cerr << e.what() << '\n';
    }

    std::cout << strlen("test") << "\n";

    std::string t1("NEED_RECORD: {:process 140183075182336, :type :invoke, :f :write, :value 202168352}");
    auto n = t1.find("NEED_RECORD: ");
    std::cout << n << " " << t1.substr(n + strlen("NEED_RECORD: ")) << "\n";

    std::cout << std::stoi("450086335") << "\n";
    std::cout << std::stoll("3876838356") << "\n";
    std::cout << std::stoll("3876838356") << "\n";
    a();
    return 0;
}
