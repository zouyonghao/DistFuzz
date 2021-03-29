
set(SOURCE_DIR src)

find_package(Threads REQUIRED)

include_directories(src)
include_directories(include)
include_directories(${CMAKE_SOURCE_DIR}/core/include)

file(GLOB BRAFT_TEST_OPERATORS_SRC
     "src/operators/*.cpp"
)

file(GLOB BRAFT_TEST_MAIN_SRC
     "src/*.cpp"
     "src/operators/*.cpp"
)

add_executable(braft_test_main ${BRAFT_TEST_MAIN_SRC})
target_link_libraries(braft_test_main dst_default_operator dst_random dst_default_kv)


add_executable(braft_operator_test ${BRAFT_TEST_OPERATORS_SRC} test/operator_test.cpp)
target_link_libraries(braft_operator_test dst_default_operator dst_random dst_default_kv)