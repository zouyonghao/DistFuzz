
set(SOURCE_DIR src)

find_package(Threads REQUIRED)
find_package(Boost REQUIRED COMPONENTS system)

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
target_link_libraries(rethinkdb_test_main dst_default_operator dst_random dst_default_kv dst_event ${Boost_LIBRARIES})


add_executable(rethinkdb_operator_test ${RETHINKDB_TEST_OPERATORS_SRC})
target_link_libraries(rethinkdb_operator_test dst_default_operator dst_random dst_default_kv dst_event ${Boost_LIBRARIES})