add_executable(test_aerospike_configuration_generator test_configuration_generator.cpp)
target_link_libraries(test_aerospike_configuration_generator pthread ${Boost_LIBRARIES})
add_test(test_aerospike_configuration_generator test_aerospike_configuration_generator)

add_executable(test_aerospike_operators test_operators.cpp)
target_link_libraries(test_aerospike_operators PUBLIC
        -Wl,--whole-archive aerospike_operator_lib dst_default_operator
        -Wl,--no-whole-archive dst_core rt pthread ${Boost_LIBRARIES} aerospike ssl crypto dl m z)
add_test(test_aerospike_operators test_aerospike_operators)
