#include <operator/dst_operator.hpp>

#include <iostream>

int main(int argc, char const *argv[])
{
    for (auto &item : Registry<CriticalOperator>::getItemMap())
    {
        std::cout << "critical operator is " << item.first << std::endl;
        // auto tmp = (item.second->_do() ? "true" : "false");
        // std::cout << "\033[1;31mreturn value is " << tmp << "\033[0m\n";
    }
    for (auto &item : Registry<NormalOperator>::getItemMap())
    {
        std::cout << "normal operator is " << item.first << std::endl;
        auto tmp = (item.second->_do() ? "true" : "false");
        std::cout << "\033[1;31mreturn value is " << tmp << "\033[0m\n";
    }
    return 0;
}
