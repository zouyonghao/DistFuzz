
set(SOURCE_DIR src)

include_directories(${CMAKE_SOURCE_DIR}/core/include)
include_directories(include)
include_directories(src)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(event_control
                ${SOURCE_DIR}/main.cpp)
target_link_libraries(event_control pthread ${Boost_LIBRARIES} rt)
target_link_options(event_control PUBLIC -static-libgcc -static)