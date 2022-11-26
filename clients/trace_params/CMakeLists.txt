cmake_minimum_required(VERSION 3.13.0)
project(trace_params LANGUAGES CXX)

# Instrumentation library
set(HEADER ../common/include/record/record_type.h ./include/record_writer.h ../common/include/utils/bira_log.h ../common/include/utils/tsc_timer.h)
#set(HEADER ./include/pmu_collector.h ../common/include/record/record_type.h ../common/include/record/record_writer.h ../common/include/utils/bira_log.h ../common/include/utils/tsc_timer.h)
#SET(BIRA_RECORD_LIB bira_record)

add_library(trace_params SHARED trace_params.cpp ${HEADER})
target_include_directories(trace_params PRIVATE ./include ../common/include)
target_link_libraries(trace_params ${BIRA_RECORD_LIB})
target_compile_options(trace_params PRIVATE ${WARNINGS})

install(TARGETS trace_params DESTINATION lib)
