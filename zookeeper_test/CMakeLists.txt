set(SOURCE_DIR src)

include_directories(src)
include_directories(include)
include_directories(${CMAKE_SOURCE_DIR}/core/include)
include_directories(${Boost_INCLUDE_DIRS})

file(GLOB ZOOKEEPER_TEST_MAIN_SRC
        "src/operators/*.cpp"
        "src/*.cpp"
        ${CMAKE_SOURCE_DIR}/core/src/dst_test_main.cpp
        )

add_executable(zookeeper_test_main ${ZOOKEEPER_TEST_MAIN_SRC})
target_compile_options(zookeeper_test_main PUBLIC -DNO_CONCURRENCY)
target_link_libraries(zookeeper_test_main
        -Wl,--whole-archive dst_default_operator
        -Wl,--no-whole-archive dst_core pthread rt ${Boost_LIBRARIES})


file(GLOB ZOOKEEPER_TEST_OPERATORS_SRC
        "src/operators/*.cpp"
        ${CMAKE_SOURCE_DIR}/core/test/dst_operator_test.cpp
        )
add_executable(zookeeper_operator_test ${ZOOKEEPER_TEST_OPERATORS_SRC})
target_link_libraries(zookeeper_operator_test
        -Wl,--whole-archive dst_default_operator
        -Wl,--no-whole-archive dst_core pthread rt ${Boost_LIBRARIES})

add_library(zookeeper_operator_lib src/operators/zookeeper_client_test.cpp)
enable_testing()
add_subdirectory(tests)
