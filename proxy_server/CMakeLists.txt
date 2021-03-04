
set(SOURCE_DIR src)

find_package(Threads REQUIRED)
include_directories(src)

add_executable(proxy_server
                ${SOURCE_DIR}/proxy_server.cpp
                ${SOURCE_DIR}/main.cpp)
target_link_libraries(proxy_server pthread)