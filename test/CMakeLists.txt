
add_executable(test1 test.cpp)

add_executable(test_network_delay_server test_network_delay_server.cpp)
add_executable(test_network_delay_client test_network_delay_client.cpp)

add_executable(test_get_fd_type test_get_fd_type.cpp)

include_directories(${CMAKE_SOURCE_DIR}/core/include)
