#include <dst_node_manager.hpp>

#include <iostream>

int main(int argc, char const *argv[])
{
    NodeManager nm("echo node id is $", 10);
    nm.start_all();
    nm.wait_all();
    // system("rm ./log*");
    return 0;
}
