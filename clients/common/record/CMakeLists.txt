#SET(CMAKE_C_COMPILER ${MPI_C_COMPILER})
#SET(CMAKE_CXX_COMPILER ${MPI_CXX_COMPILER})

SET(HEADER ../include/record/record_type.h ../include/record/record_writer.h)
add_library(bira_record SHARED record_writer.cpp ${HEADER})
target_include_directories(bira_record PRIVATE ../include)
#target_link_libraries(bira_record ${LIBUNWIND_LIBRARIES} ${PAPI_LIBRARIES} dl)

install(TARGETS bira_record DESTINATION lib)
