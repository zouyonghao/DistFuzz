
set(SOURCE_DIR src)

find_package(Threads REQUIRED)
include_directories(include)
include_directories(src)

add_library(restart_operator SHARED src/restart_operator.cpp)