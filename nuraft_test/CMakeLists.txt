set(SOURCE_DIR src)

include_directories(src)
include_directories(include)
include_directories(${CMAKE_SOURCE_DIR}/core/include)
include_directories(${Boost_INCLUDE_DIRS})

file(GLOB NURAFT_TEST_OPERATORS_SRC
        "src/operators/*.cpp"
        ${CMAKE_SOURCE_DIR}/core/test/dst_operator_test.cpp
        )

file(GLOB NURAFT_TEST_MAIN_SRC
        "src/*.cpp"
        "src/operators/*.cpp"
        ${CMAKE_SOURCE_DIR}/core/src/dst_test_main.cpp
        )

add_executable(nuraft_test_main ${NURAFT_TEST_MAIN_SRC})
target_compile_definitions(nuraft_test_main PUBLIC -DNO_CONCURRENCY)
target_link_libraries(nuraft_test_main
        -Wl,--whole-archive dst_default_operator
        -Wl,--no-whole-archive dst_core rt pthread ${Boost_LIBRARIES})


add_executable(nuraft_operator_test ${NURAFT_TEST_OPERATORS_SRC})
target_link_libraries(nuraft_operator_test
        -Wl,--whole-archive dst_default_operator
        -Wl,--no-whole-archive dst_core rt pthread ${Boost_LIBRARIES})

add_library(nuraft_operator_lib src/operators/nuraft_client.cpp)
enable_testing()
add_subdirectory(tests)