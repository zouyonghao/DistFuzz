
set(SOURCE_DIR src)

find_package(Threads REQUIRED)
find_package(Boost REQUIRED COMPONENTS)

include_directories(${CMAKE_SOURCE_DIR}/core/include)
include_directories(include)
include_directories(src)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(event_control
                ${SOURCE_DIR}/main.cpp)
target_link_libraries(event_control pthread ${Boost_LIBRARIES} rt)