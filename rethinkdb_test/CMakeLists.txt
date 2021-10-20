set(SOURCE_DIR src)

include_directories(src)
include_directories(include)
include_directories(${CMAKE_SOURCE_DIR}/core/include)
include_directories(${Boost_INCLUDE_DIRS})

file(GLOB RETHINKDB_TEST_OPERATORS_SRC
        "src/operators/*.cpp"
        ${CMAKE_SOURCE_DIR}/core/test/dst_operator_test.cpp
        )

file(GLOB RETHINKDB_TEST_MAIN_SRC
        "src/operators/*.cpp"
        "src/*.cpp"
        ${CMAKE_SOURCE_DIR}/core/src/dst_test_main.cpp
        )

add_executable(rethinkdb_test_main ${RETHINKDB_TEST_MAIN_SRC})
target_compile_options(rethinkdb_test_main PUBLIC -DNO_CONCURRENCY)
target_link_libraries(rethinkdb_test_main
        -Wl,--whole-archive dst_default_operator
        -Wl,--no-whole-archive dst_core pthread rt ${Boost_LIBRARIES})

add_executable(rethinkdb_operator_test ${RETHINKDB_TEST_OPERATORS_SRC})
target_link_libraries(rethinkdb_operator_test
        -Wl,--whole-archive dst_default_operator
        -Wl,--no-whole-archive dst_core pthread rt ${Boost_LIBRARIES})

add_library(rethinkdb_operator_lib src/operators/rethinkdb_client_test.cpp)
enable_testing()
add_subdirectory(tests)
