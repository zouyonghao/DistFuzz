
set(SOURCE_DIR src)

find_package(Threads REQUIRED)

include_directories(src)
include_directories(include)
include_directories(${CMAKE_SOURCE_DIR}/core/include)

file(GLOB braft_test_main_SRC
     "src/*.cpp"
     "src/operators/*.cpp"
)

add_executable(braft_test_main ${braft_test_main_SRC})

target_link_libraries(braft_test_main default_operator dst_random dst_default_kv)