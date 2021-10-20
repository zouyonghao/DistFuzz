add_executable(test_redisraft_configuration_generator test_configuration_generator.cpp)
target_link_libraries(test_redisraft_configuration_generator ${Boost_LIBRARIES} pthread)
add_test(test_redisraft_configuration_generator test_redisraft_configuration_generator)

add_executable(test_redisraft_operators test_operators.cpp)
target_link_libraries(test_redisraft_operators PUBLIC
        -Wl,--whole-archive redisraft_operator_lib dst_default_operator
        -Wl,--no-whole-archive dst_core pthread rt ${Boost_LIBRARIES})
add_test(test_redisraft_operators test_redisraft_operators)