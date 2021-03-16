
set(SOURCE_DIR src)

find_package(Threads REQUIRED)

include_directories(src)
include_directories(${CMAKE_SOURCE_DIR}/core/include)

add_executable(braft_test_main ${SOURCE_DIR}/braft_test_main.cpp)

target_link_libraries(braft_test_main restart_operator) # add restart operator