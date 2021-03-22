
set(SOURCE_DIR src)

find_package(Threads REQUIRED)
find_package(Boost REQUIRED COMPONENTS)

include_directories(include)
include_directories(src)
include_directories(${Boost_INCLUDE_DIRS})

add_library(default_operator SHARED src/dst_default_operator.cpp)

add_library(dst_event SHARED src/dst_default_event.cpp)
target_link_libraries(dst_event pthread ${Boost_LIBRARIES} rt)

add_library(dst_random SHARED src/dst_default_random.c)
target_link_libraries(dst_random pthread ${Boost_LIBRARIES} rt)

add_library(dst_default_kv SHARED src/dst_default_in_memory_kv_store.cpp)
target_link_libraries(dst_default_kv pthread ${Boost_LIBRARIES} rt)

# unit test
add_executable(test_dst_default_kv test/dst_test_kv.cpp)
target_link_libraries(test_dst_default_kv pthread ${Boost_LIBRARIES} rt dst_default_kv)