set(SOURCE_DIR src)

include_directories(src)
include_directories(include)
include_directories(${CMAKE_SOURCE_DIR}/core/include)
include_directories(${Boost_INCLUDE_DIRS})

file(GLOB DQLITE_TEST_MAIN_SRC
        "src/*.cpp"
        "src/operators/*.cpp"
        ${CMAKE_SOURCE_DIR}/core/src/dst_test_main.cpp
        )

add_executable(dqlite_test_main ${DQLITE_TEST_MAIN_SRC})
target_link_libraries(dqlite_test_main
        -Wl,--whole-archive dst_default_operator
        -Wl,--no-whole-archive dst_core rt pthread ${Boost_LIBRARIES})
target_compile_options(dqlite_test_main PUBLIC -DNO_CONCURRENCY)
