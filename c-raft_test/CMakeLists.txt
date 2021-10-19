set(SOURCE_DIR src)

include_directories(src)
include_directories(include)
include_directories(${CMAKE_SOURCE_DIR}/core/include)
include_directories(${Boost_INCLUDE_DIRS})

file(GLOB CRAFT_TEST_OPERATORS_SRC
     "src/operators/*.cpp"
     ${CMAKE_SOURCE_DIR}/core/test/dst_operator_test.cpp
)

file(GLOB CRAFT_TEST_MAIN_SRC
     "src/*.cpp"
     "src/operators/*.cpp"
     ${CMAKE_SOURCE_DIR}/core/src/dst_test_main.cpp
)

add_executable(c-raft_test_main ${CRAFT_TEST_MAIN_SRC})
target_link_libraries(c-raft_test_main
        -Wl,--whole-archive dst_default_operator
        -Wl,--no-whole-archive dst_random dst_default_kv dst_event ${Boost_LIBRARIES})
target_compile_options(c-raft_test_main PUBLIC -DNO_CONCURRENCY)

add_executable(c-raft_operator_test ${CRAFT_TEST_OPERATORS_SRC})
target_link_libraries(c-raft_operator_test
        -Wl,--whole-archive dst_default_operator
        -Wl,--no-whole-archive dst_random dst_default_kv dst_event ${Boost_LIBRARIES})

add_library(craft_operator_lib src/operators/sleep_operator.cpp)
enable_testing()
add_subdirectory(tests)
