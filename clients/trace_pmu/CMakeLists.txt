cmake_minimum_required(VERSION 3.13.0)
project(trace_pmu LANGUAGES CXX)

# Instrumentation library
set(HEADER ./include/pmu_collector.h)
add_library(trace_pmu SHARED trace_pmu.cpp pmu_collector.cpp ${HEADER})
target_include_directories(trace_pmu PRIVATE ./include)
target_link_libraries(trace_pmu papi)
target_compile_options(trace_pmu PRIVATE ${WARNINGS})

