add_executable(test_zookeeper_configuration_generator test_configuration_generator.cpp)
target_link_libraries(test_zookeeper_configuration_generator dst_core ${Boost_LIBRARIES} pthread)
add_test(test_zookeeper_configuration_generator test_zookeeper_configuration_generator)

add_executable(test_zooclickhouse_operators test_operators.cpp)
target_link_libraries(test_zooclickhouse_operators PUBLIC
        -Wl,--whole-archive clickhouse_operator_lib dst_default_operator
        -Wl,--no-whole-archive dst_core pthread rt ${Boost_LIBRARIES})
add_test(test_zooclickhouse_operators test_zooclickhouse_operators)
