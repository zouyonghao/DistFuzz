#include <dst_operator.hpp>
#include <dst_operator_registry.hpp>

#include <iostream>

int main(int argc, char const *argv[])
{
    for (auto &item : OperatorRegistry<Operator>::getItemMap())
    {
        std::cout << "item name is " << item.first << std::endl;
        auto tmp = (item.second->_do() ? "true" : "false");
        std::cout << "\033[1;31mreturn value is " << tmp << "\033[0m\n";
    }
    return 0;
}
