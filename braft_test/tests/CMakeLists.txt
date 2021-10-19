add_executable(test_braft_configuration_generator test_configuration_generator.cpp)
target_link_libraries(test_braft_configuration_generator ${Boost_LIBRARIES})
add_test(test_braft_configuration_generator test_braft_configuration_generator)

add_executable(test_braft_operators test_operators.cpp)
target_link_libraries(test_braft_operators PUBLIC
        -Wl,--whole-archive braft_operator_lib dst_default_operator
        -Wl,--no-whole-archive ${Boost_LIBRARIES} dst_random dst_event pthread)
add_test(test_braft_operators test_braft_operators)
