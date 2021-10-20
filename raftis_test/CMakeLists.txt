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
target_link_libraries(raftis_test_main
        -Wl,--whole-archive dst_default_operator
        -Wl,--no-whole-archive dst_core pthread rt
        ${Boost_LIBRARIES})

add_executable(raftis_operator_test ${RAFTIS_TEST_OPERATORS_SRC})
target_link_libraries(raftis_operator_test
        -Wl,--whole-archive dst_default_operator
        -Wl,--no-whole-archive dst_core pthread rt
        ${Boost_LIBRARIES})