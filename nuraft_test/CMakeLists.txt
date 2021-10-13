set(SOURCE_DIR src)

include_directories(src)
include_directories(include)
include_directories(${CMAKE_SOURCE_DIR}/core/include)
include_directories(${Boost_INCLUDE_DIRS})

file(GLOB NURAFT_TEST_OPERATORS_SRC
     "src/operators/*.cpp"
     ${CMAKE_SOURCE_DIR}/core/test/dst_operator_test.cpp
)

file(GLOB NURAFT_TEST_MAIN_SRC
     "src/*.cpp"
     "src/operators/*.cpp"
     ${CMAKE_SOURCE_DIR}/core/src/dst_test_main.cpp
)

add_executable(nuraft_test_main ${NURAFT_TEST_MAIN_SRC})
target_compile_definitions(nuraft_test_main PUBLIC -DNO_CONCURRENCY)
target_link_libraries(nuraft_test_main dst_default_operator dst_random dst_default_kv dst_event ${Boost_LIBRARIES})


add_executable(nuraft_operator_test ${NURAFT_TEST_OPERATORS_SRC})
target_link_libraries(nuraft_operator_test dst_default_operator dst_random dst_default_kv dst_event ${Boost_LIBRARIES})

enable_testing()
add_subdirectory(tests)