set(SOURCE_DIR src)

include_directories(src)
include_directories(${CMAKE_SOURCE_DIR}/core/include)

include_directories(include)

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
