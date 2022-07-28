add_executable(test_etcd_configuration_generator test_configuration_generator.cpp)
target_link_libraries(test_etcd_configuration_generator dst_core ${Boost_LIBRARIES} pthread)
add_test(test_etcd_configuration_generator test_etcd_configuration_generator)

add_executable(test_etcd_operators test_operators.cpp)
target_link_libraries(test_etcd_operators PUBLIC
        -Wl,--whole-archive etcd_operator_lib dst_default_operator
        -Wl,--no-whole-archive dst_core rt pthread ${Boost_LIBRARIES})
add_test(test_etcd_operators test_etcd_operators)
