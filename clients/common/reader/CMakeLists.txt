SET(HEADER ../include/record/record_type.h ../include/record/record_reader.h)
add_library(bira_reader SHARED record_reader.cpp ${HEADER})
target_compile_features(bira_reader PRIVATE cxx_std_17)
target_include_directories(bira_reader PRIVATE ../include)
target_link_libraries(bira_reader dl)


install(TARGETS bira_reader DESTINATION lib)
