add_executable(test_keeper_configuration_generator test_configuration_generator.cpp)
target_link_libraries(test_keeper_configuration_generator ${Boost_LIBRARIES} pthread)
add_test(test_keeper_configuration_generator test_keeper_configuration_generator)

add_executable(test_clickhouse_operators test_operators.cpp)
target_link_libraries(test_clickhouse_operators PUBLIC
        -Wl,--whole-archive clickhouse_operator_lib dst_default_operator
        -Wl,--no-whole-archive ${Boost_LIBRARIES} dst_random dst_event pthread)
add_test(test_clickhouse_operators test_clickhouse_operators)
