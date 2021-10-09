
set(SOURCE_DIR src)

include_directories(src)
include_directories(include)
include_directories(${CMAKE_SOURCE_DIR}/core/include)
include_directories(${Boost_INCLUDE_DIRS})

file(GLOB RAFTIS_TEST_OPERATORS_SRC
     "src/operators/*.cpp"
     ${CMAKE_SOURCE_DIR}/core/test/dst_operator_test.cpp
)

file(GLOB RAFTIS_TEST_MAIN_SRC
     "src/*.cpp"
     "src/operators/*.cpp"
     ${CMAKE_SOURCE_DIR}/core/src/dst_test_main.cpp
)

add_executable(raftis_test_main ${RAFTIS_TEST_MAIN_SRC})
target_link_libraries(raftis_test_main dst_default_operator dst_random dst_default_kv dst_event ${Boost_LIBRARIES})

add_executable(raftis_operator_test ${RAFTIS_TEST_OPERATORS_SRC})
target_link_libraries(raftis_operator_test dst_default_operator dst_random dst_default_kv dst_event ${Boost_LIBRARIES})