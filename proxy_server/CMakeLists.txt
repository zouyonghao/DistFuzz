
set(SOURCE_DIR src)

include_directories(src)
include_directories(${CMAKE_SOURCE_DIR}/core/include)

add_executable(proxy_server
                ${SOURCE_DIR}/proxy_server.cpp
                ${SOURCE_DIR}/main.cpp)
target_link_libraries(proxy_server pthread dst_event dst_random)