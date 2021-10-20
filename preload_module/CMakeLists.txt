
include_directories(${CMAKE_SOURCE_DIR}/core/include)
add_library(dst_preload SHARED src/preload.cpp)
target_compile_options(dst_preload PUBLIC -fPIC)
target_link_libraries(dst_preload dl dst_random dst_utils)
