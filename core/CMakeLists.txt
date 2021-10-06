
set(SOURCE_DIR src)

find_package(Threads REQUIRED)
find_package(Boost REQUIRED COMPONENTS system)

include_directories(include)
include_directories(src)
include_directories(${Boost_INCLUDE_DIRS})

add_library(dst_default_operator SHARED
            src/operator/dst_default_operator.cpp
            src/operator/dst_kill_by_name.cpp
            src/operator/dst_start_by_name.cpp
            src/operator/dst_default_client_operator.cpp)
target_compile_options(dst_default_operator PUBLIC -fPIC)

add_library(dst_event SHARED src/dst_default_event.cpp)
target_link_libraries(dst_event pthread ${Boost_LIBRARIES} rt)
target_compile_options(dst_event PUBLIC -fPIC)

add_library(dst_random SHARED src/dst_default_random.c)
target_link_libraries(dst_random pthread ${Boost_LIBRARIES} rt)
target_compile_options(dst_random PUBLIC -fPIC)

add_library(dst_default_kv SHARED src/dst_default_in_memory_kv_store.cpp)
target_link_libraries(dst_default_kv pthread ${Boost_LIBRARIES} rt)
target_compile_options(dst_default_kv PUBLIC -fPIC)

# unit test
add_executable(dst_test_default_kv test/dst_test_kv.cpp)
target_link_libraries(dst_test_default_kv pthread ${Boost_LIBRARIES} rt dst_default_kv)

add_executable(dst_test_node_manager test/dst_test_node_manager.cpp)
target_link_libraries(dst_test_node_manager pthread rt dst_default_kv ${Boost_LIBRARIES})