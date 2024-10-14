set(SOURCE_DIR src)

include_directories(src)
include_directories(include)
include_directories(${CMAKE_SOURCE_DIR}/core/include)
include_directories(${Boost_INCLUDE_DIRS})

file(GLOB AEROSPIKE_TEST_MAIN_SRC
        "src/*.cpp"
        "src/operators/*.cpp"
        ${CMAKE_SOURCE_DIR}/core/src/dst_test_main.cpp
        )

add_executable(aerospike_test_main ${AEROSPIKE_TEST_MAIN_SRC})
target_compile_options(aerospike_test_main PUBLIC -DNO_CONCURRENCY -DRUN_NORMAL_OPERATOR_COUNT=5 -DRUN_CRITICAL_OPERATOR_COUNT=3)
target_link_libraries(aerospike_test_main
        -Wl,--whole-archive dst_default_operator
        -Wl,--no-whole-archive dst_core rt pthread aerospike ssl crypto dl m z
        ${Boost_LIBRARIES})

file(GLOB AEROSPIKE_TEST_OPERATORS_SRC
        "src/operators/*.cpp"
        )
add_executable(aerospike_operator_test ${AEROSPIKE_TEST_OPERATORS_SRC} ${CMAKE_SOURCE_DIR}/core/test/dst_operator_test.cpp)
target_link_libraries(aerospike_operator_test
        -Wl,--whole-archive dst_default_operator
        -Wl,--no-whole-archive dst_core rt pthread aerospike ssl crypto dl m z
        ${Boost_LIBRARIES})

file(GLOB AEROSPIKE_CLIENT_TEST_SRC
        "test/test_client.cpp"
        )

add_executable(aerospike_client_test ${AEROSPIKE_CLIENT_TEST_SRC})
target_link_libraries(aerospike_client_test
        -Wl,--whole-archive dst_default_operator
        # we do not statically link dst_core because a random function is rewrite in test_client.cpp
        -Wl,--no-whole-archive dst_random dst_default_kv dst_event aerospike ssl crypto dl m z
        ${Boost_LIBRARIES})

add_executable(example_client src/example_client/example_client.cpp)
target_link_libraries(example_client
        -Wl,--whole-archive dst_default_operator
        # we do not statically link dst_core because a random function is rewrite in test_client.cpp
        -Wl,--no-whole-archive dst_random dst_default_kv dst_event aerospike ssl crypto dl m z
        ${Boost_LIBRARIES})

add_executable(test_client src/example_client/test_client.cpp)
target_link_libraries(test_client
        -Wl,--whole-archive dst_default_operator
        -Wl,--no-whole-archive dst_random dst_default_kv dst_event aerospike ssl crypto dl m z
        ${Boost_LIBRARIES})

add_library(aerospike_operator_lib ${AEROSPIKE_TEST_OPERATORS_SRC})
enable_testing()
add_subdirectory(tests)
