add_executable(test_etcd_configuration_generator-3517 test_configuration_generator.cpp)
target_link_libraries(test_etcd_configuration_generator-3517 dst_core ${Boost_LIBRARIES} pthread)
add_test(test_etcd_configuration_generator-3517 test_etcd_configuration_generator-3517)

add_executable(test_etcd_operators-3517 test_operators.cpp)
target_link_libraries(test_etcd_operators-3517 PUBLIC
        -Wl,--whole-archive etcd_operator_lib dst_default_operator
        -Wl,--no-whole-archive dst_core rt pthread ${Boost_LIBRARIES})
add_test(test_etcd_operators-3517 test_etcd_operators-3517)
