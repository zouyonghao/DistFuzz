set(SOURCE_DIR src)

include_directories(src)
include_directories(include)
include_directories(${CMAKE_SOURCE_DIR}/core/include)
include_directories(${Boost_INCLUDE_DIRS})

file(GLOB BRAFT_TEST_OPERATORS_SRC
        "src/operators/*.cpp"
        ${CMAKE_SOURCE_DIR}/core/test/dst_operator_test.cpp
        )

file(GLOB BRAFT_TEST_MAIN_SRC
        "src/node_manager.cpp"
        "src/operators/*.cpp"
        ${CMAKE_SOURCE_DIR}/core/src/dst_test_main.cpp
        )

add_executable(braft_test_main ${BRAFT_TEST_MAIN_SRC})
target_link_libraries(braft_test_main
        -Wl,--whole-archive dst_default_operator
        -Wl,--no-whole-archive dst_core pthread rt ${Boost_LIBRARIES})
target_compile_options(braft_test_main PUBLIC -DNO_CONCURRENCY)

add_executable(braft_operator_test ${BRAFT_TEST_OPERATORS_SRC})
target_link_libraries(braft_operator_test
        -Wl,--whole-archive dst_default_operator
        -Wl,--no-whole-archive dst_core pthread rt ${Boost_LIBRARIES})

add_library(braft_operator_lib src/operators/atomic_test.cpp)
enable_testing()
add_subdirectory(tests)