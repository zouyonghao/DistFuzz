
set(SOURCE_DIR src)

find_package(Threads REQUIRED)
include_directories(include)
include_directories(src)

add_library(restart_operator SHARED src/restart_operator.cpp)


find_package(Boost REQUIRED COMPONENTS)

include_directories(${Boost_INCLUDE_DIRS})

add_library(dst_event SHARED src/default_event.cpp)
target_link_libraries(dst_event pthread ${Boost_LIBRARIES} rt)