set(SOURCE_DIR src)

include_directories(include)
include_directories(src)
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/third-party/diff-match-patch)

add_library(dst_event SHARED src/dst_default_event.cpp)
target_link_libraries(dst_event rt pthread ${Boost_LIBRARIES})
target_compile_options(dst_event PUBLIC -fPIC)

add_library(dst_random SHARED src/dst_default_random.c)
target_link_libraries(dst_random rt pthread ${Boost_LIBRARIES})
target_compile_options(dst_random PUBLIC -fPIC)

add_library(dst_default_kv SHARED src/dst_default_in_memory_kv_store.cpp)
target_link_libraries(dst_default_kv rt pthread ${Boost_LIBRARIES})
target_compile_options(dst_default_kv PUBLIC -fPIC)

add_library(dst_default_operator
        src/operator/dst_default_operator.cpp
        src/operator/dst_random_kill.cpp
        src/operator/dst_random_start.cpp
        src/operator/dst_default_client_operator.cpp)

add_library(dst_utils SHARED
        src/utils/share_mem_util.cpp)
target_link_libraries(dst_utils rt pthread ${Boost_LIBRARIES})
target_compile_options(dst_utils PUBLIC -fPIC)

# dst core consists of dst_event, dst_random, dst_default_kv, dst_utils
add_library(dst_core
        src/dst_default_event.cpp
        src/dst_default_random.c
        src/dst_default_in_memory_kv_store.cpp
        src/utils/share_mem_util.cpp)
target_compile_options(dst_core PUBLIC -fPIC)

# unit test
add_executable(dst_test_default_kv test/dst_test_kv.cpp)
target_link_libraries(dst_test_default_kv rt pthread ${Boost_LIBRARIES} dst_default_kv)

add_executable(dst_test_node_manager test/dst_test_node_manager.cpp)
target_link_libraries(dst_test_node_manager rt pthread dst_default_kv ${Boost_LIBRARIES})

add_executable(dst_test_diff test/dst_test_diff.cpp)
target_compile_options(dst_test_diff PUBLIC -fsanitize=address)
target_link_libraries(dst_test_diff rt pthread dst_core ${Boost_LIBRARIES})
target_link_options(dst_test_diff PUBLIC -fsanitize=address)