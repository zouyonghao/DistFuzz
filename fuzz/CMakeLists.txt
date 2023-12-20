set(SOURCE_DIR src)

include_directories(src)
include_directories(${CMAKE_SOURCE_DIR}/core/include)

include_directories(include)

if (${DIST_FUZZ_BUILD_COMPILER})
set(LLVM_DIST_PATH "/usr/lib/llvm-11"
        CACHE STRING "LLVM distribution install path")

set(LLVM_DIR ${LLVM_DIST_PATH}/lib/cmake/llvm)
find_package(LLVM REQUIRED CONFIG)
message(STATUS "Using LLVM Version ${LLVM_PACKAGE_VERSION}")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})
# Handle RTTI stuff, which often leads to error
if (NOT ${LLVM_ENABLE_RTTI})
    if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang" OR
            "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
            "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
        message(STATUS "Disable RTTI")
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /GR-")
        message(STATUS "Disable RTTI")
    endif ()
    # Do not give any flags for other less widely used
    # compilers
endif ()
link_directories(${LLVM_LIBRARY_DIRS})

llvm_map_components_to_libnames(LLVM_LIBS bitreader bitwriter interpreter core irreader mcjit native option support)

add_library(default_instrumentor instrumentor/default_instrumentor.cpp)
target_link_libraries(default_instrumentor ${LLVM_LIBS})

add_executable(default_compiler compiler/compiler.cpp instrumentor/default_instrumentor.cpp instrumentor/process_concurrent_coverage_instrumentor.cpp
       instrumentor/process_tick_instrumentor.cpp
       instrumentor/process_function_sequence_instrumentor.cpp)
target_compile_definitions(default_compiler PUBLIC -DCLANG="clang-11")
target_link_libraries(default_compiler ${LLVM_LIBS} ${Boost_LIBRARIES})

add_executable(default_compiler++ compiler/compiler.cpp instrumentor/default_instrumentor.cpp instrumentor/process_concurrent_coverage_instrumentor.cpp
       instrumentor/process_tick_instrumentor.cpp
       instrumentor/process_function_sequence_instrumentor.cpp)
target_compile_definitions(default_compiler++ PUBLIC -DCLANG="clang++-11")
target_link_libraries(default_compiler++ ${LLVM_LIBS} ${Boost_LIBRARIES})

add_library(default_instrument_lib OBJECT instrumentor_lib/default_instrument_lib.c)
target_compile_options(default_instrument_lib PUBLIC -fPIC)
endif()

file(GLOB FUZZER_SRC
        "fuzzer/old/*.cpp"
        )

add_executable(fuzzer ${FUZZER_SRC})
target_compile_definitions(fuzzer PUBLIC -DNO_FUZZING_ERROR -DNO_CONCURRENCY_FUZZ)
target_link_libraries(fuzzer pthread)
if (${DIST_FUZZ_STATIC_BUILD})
target_link_options(fuzzer PUBLIC -static-libgcc -static)
endif()

if (${DIST_FUZZ_BUILD_COVERAGE_COLLECTOR})
add_executable(coverage_collector coverage_collector/coverage_collector.c)
target_link_libraries(coverage_collector rdkafka)
endif()
