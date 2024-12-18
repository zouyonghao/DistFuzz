set(SOURCE_DIR src)

include_directories(src)
include_directories(include)
include_directories(${CMAKE_SOURCE_DIR}/core/include)
include_directories(${Boost_INCLUDE_DIRS})

file(GLOB CLICKHOUSE_TEST_MAIN_SRC
        "src/*.cpp"
        "src/keeper_operators/*.cpp"
        ${CMAKE_SOURCE_DIR}/core/src/dst_test_main.cpp
        )

add_executable(clickhouse_test_main ${CLICKHOUSE_TEST_MAIN_SRC})
target_link_libraries(clickhouse_test_main
        -Wl,--whole-archive dst_default_operator
        -Wl,--no-whole-archive dst_core rt pthread
        ${Boost_LIBRARIES})
target_compile_options(clickhouse_test_main PUBLIC -DNO_CONCURRENCY -DRUN_NORMAL_OPERATOR_COUNT=5 -DRUN_CRITICAL_OPERATOR_COUNT=3)

file(GLOB CLICKHOUSE_TEST_OPERATORS_SRC
        "src/keeper_operators/*.cpp"
        ${CMAKE_SOURCE_DIR}/core/test/dst_operator_test.cpp
        )

add_executable(clickhouse_operator_test ${CLICKHOUSE_TEST_OPERATORS_SRC})
target_link_libraries(clickhouse_operator_test
        -Wl,--whole-archive dst_default_operator
        -Wl,--no-whole-archive dst_core rt pthread
        ${Boost_LIBRARIES})

add_library(clickhouse_operator_lib src/keeper_operators/delete_data_folder.cpp src/keeper_operators/keeper_client_test.cpp)
enable_testing()
add_subdirectory(tests)
