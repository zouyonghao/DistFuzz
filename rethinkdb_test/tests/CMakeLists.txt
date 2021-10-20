add_executable(test_rethinkdb_configuration_generator test_configuration_generator.cpp)
target_link_libraries(test_rethinkdb_configuration_generator ${Boost_LIBRARIES} pthread)
add_test(test_rethinkdb_configuration_generator test_rethinkdb_configuration_generator)

add_executable(test_rethinkdb_operators test_operators.cpp)
target_link_libraries(test_rethinkdb_operators PUBLIC
        -Wl,--whole-archive rethinkdb_operator_lib dst_default_operator
        -Wl,--no-whole-archive dst_core pthread rt ${Boost_LIBRARIES})
add_test(test_rethinkdb_operators test_rethinkdb_operators)