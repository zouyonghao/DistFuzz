set(SOURCE_DIR src)

include_directories(src)
include_directories(include)
include_directories(${CMAKE_SOURCE_DIR}/core/include)
include_directories(${Boost_INCLUDE_DIRS})

file(GLOB GENERAL_TEST_MAIN_SRC
        "src/operators/*.cpp"
        "src/*.cpp"
        ${CMAKE_SOURCE_DIR}/core/src/dst_test_main.cpp
        )

add_executable(general_test_main ${GENERAL_TEST_MAIN_SRC})
target_compile_options(general_test_main PUBLIC -DNO_CONCURRENCY)
target_link_libraries(general_test_main
        -Wl,--whole-archive dst_default_operator
        -Wl,--no-whole-archive dst_core pthread rt ${Boost_LIBRARIES})


file(GLOB GENERAL_TEST_OPERATORS_SRC
        "src/operators/*.cpp"
        ${CMAKE_SOURCE_DIR}/core/test/dst_operator_test.cpp
        )
add_executable(general_operator_test ${GENERAL_TEST_OPERATORS_SRC})
target_link_libraries(general_operator_test
        -Wl,--whole-archive dst_default_operator
        -Wl,--no-whole-archive dst_core pthread rt ${Boost_LIBRARIES})

add_library(general_operator_lib src/operators/general_client_test.cpp)
enable_testing()
add_subdirectory(tests)
