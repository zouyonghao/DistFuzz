set(SOURCE_DIR src)

include_directories(src)
include_directories(include)
include_directories(${CMAKE_SOURCE_DIR}/core/include)
include_directories(${Boost_INCLUDE_DIRS})

file(GLOB AEROSPIKE_TEST_OPERATORS_SRC
     "src/operators/*.cpp"
     ${CMAKE_SOURCE_DIR}/core/test/dst_operator_test.cpp
)

file(GLOB AEROSPIKE_TEST_MAIN_SRC
     "src/*.cpp"
     "src/operators/*.cpp"
     ${CMAKE_SOURCE_DIR}/core/src/dst_test_main.cpp
)

add_executable(aerospike_test_main ${AEROSPIKE_TEST_MAIN_SRC})
target_link_libraries(aerospike_test_main
     dst_default_operator dst_random dst_default_kv dst_event
     aerospike ssl crypto dl m z
     ${Boost_LIBRARIES})

add_executable(aerospike_operator_test ${AEROSPIKE_TEST_OPERATORS_SRC})
target_link_libraries(aerospike_operator_test
     dst_default_operator dst_random dst_default_kv dst_event
     aerospike ssl crypto dl m z
     ${Boost_LIBRARIES})

file(GLOB AEROSPIKE_CLIENT_TEST_SRC
     "test/test_client.cpp"
)

add_executable(aerospike_client_test ${AEROSPIKE_CLIENT_TEST_SRC})
target_link_libraries(aerospike_client_test
     dst_default_operator dst_default_kv dst_event
     aerospike ssl crypto dl m z
     ${Boost_LIBRARIES})
