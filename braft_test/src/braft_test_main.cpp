#include <dst_operator.hpp>
#include <dst_operator_registry.hpp>

#include <iostream>

int main(int argc, char const *argv[])
{
    for (auto &item : OperatorRegistry<Operator>::getItemMap())
    {
        std::cout << "item name is " << item.first << std::endl;
        std::cout << "return value is "
                  << (item.second->_do() ? "true" : "false") << "\n";
    }
    return 0;
}
