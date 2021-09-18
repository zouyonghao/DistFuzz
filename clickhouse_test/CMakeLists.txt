
set(SOURCE_DIR src)

find_package(Threads REQUIRED)
find_package(Boost REQUIRED COMPONENTS system)

include_directories(src)
include_directories(include)
include_directories(${CMAKE_SOURCE_DIR}/core/include)
include_directories(${Boost_INCLUDE_DIRS})

file(GLOB CLICKHOUSE_TEST_MAIN_SRC
     "src/*.cpp"
     "src/operators/*.cpp"
     ${CMAKE_SOURCE_DIR}/core/src/dst_test_main.cpp
)

add_executable(clickhouse_test_main ${CLICKHOUSE_TEST_MAIN_SRC})
target_link_libraries(clickhouse_test_main
     dst_default_operator dst_random dst_default_kv dst_event
     ${Boost_LIBRARIES})

file(GLOB CLICKHOUSE_TEST_OPERATORS_SRC
     "src/keeper_operators/*.cpp"
     ${CMAKE_SOURCE_DIR}/core/test/dst_operator_test.cpp
)

add_executable(clickhouse_operator_test ${CLICKHOUSE_TEST_OPERATORS_SRC})
target_link_libraries(clickhouse_operator_test
     dst_default_operator dst_random dst_default_kv dst_event
     ${Boost_LIBRARIES})
