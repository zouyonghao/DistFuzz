cmake_minimum_required(VERSION 3.4.3)
project(distributed_system_test)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(proxy_server)
add_subdirectory(event_control)
add_subdirectory(core)
add_subdirectory(braft_test)
add_subdirectory(fuzz)