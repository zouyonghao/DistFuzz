set(SOURCE_DIR src)

include_directories(src)
include_directories(include)
include_directories(${CMAKE_SOURCE_DIR}/core/include)
include_directories(${Boost_INCLUDE_DIRS})

file(GLOB HDFS_TEST_MAIN_SRC
        "src/operators/*.cpp"
        "src/*.cpp"
        ${CMAKE_SOURCE_DIR}/core/src/dst_test_main.cpp
        )

add_executable(hdfs_test_main ${HDFS_TEST_MAIN_SRC})
target_compile_options(hdfs_test_main PUBLIC -DNO_CONCURRENCY)
target_link_libraries(hdfs_test_main
        -Wl,--whole-archive dst_default_operator
        -Wl,--no-whole-archive dst_core rt pthread ${Boost_LIBRARIES})

# add_subdirectory(tests)
