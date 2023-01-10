cmake_minimum_required(VERSION 3.13.0)
project(trace_pmu_retrowrite LANGUAGES CXX)

# Instrumentation library
set(HEADER ./include/pmu_collector.h ../common/include/record/record_type.h ./include/record_writer.h ../common/include/utils/bira_log.h ../common/include/utils/tsc_timer.h)
#set(HEADER ./include/pmu_collector.h ../common/include/record/record_type.h ../common/include/record/record_writer.h ../common/include/utils/bira_log.h ../common/include/utils/tsc_timer.h)
#SET(BIRA_RECORD_LIB bira_record)

add_library(trace_pmu_retrowrite SHARED trace_pmu_retrowrite.cpp pmu_collector.cpp ${HEADER})
target_include_directories(trace_pmu_retrowrite PRIVATE ./include ../common/include ${PAPI_INCLUDE_DIRS})
target_link_libraries(trace_pmu_retrowrite ${PAPI_LIBRARIES} ${BIRA_RECORD_LIB})
target_compile_options(trace_pmu_retrowrite PRIVATE ${WARNINGS})

install(TARGETS trace_pmu_retrowrite DESTINATION lib)
