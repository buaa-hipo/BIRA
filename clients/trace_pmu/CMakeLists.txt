cmake_minimum_required(VERSION 3.13.0)
project(trace_pmu LANGUAGES CXX)

# Instrumentation library
set(HEADER ./include/pmu_collector.h ../common/include/record/record_type.h ./include/record_writer.h ../common/include/utils/bira_log.h ../common/include/utils/tsc_timer.h)
#set(HEADER ./include/pmu_collector.h ../common/include/record/record_type.h ../common/include/record/record_writer.h ../common/include/utils/bira_log.h ../common/include/utils/tsc_timer.h)
#SET(BIRA_RECORD_LIB bira_record)

add_library(trace_pmu SHARED trace_pmu.cpp pmu_collector.cpp ${HEADER})
target_include_directories(trace_pmu PRIVATE ./include ../common/include)
target_link_libraries(trace_pmu papi ${BIRA_RECORD_LIB})
target_compile_options(trace_pmu PRIVATE ${WARNINGS})

install(TARGETS trace_pmu DESTINATION lib)
