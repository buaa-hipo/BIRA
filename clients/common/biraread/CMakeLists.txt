set(BIRA_INCLUDE ../include)
set(BIRA_LIBRARY bira_reader)

SET(HEADER ${BIRA_INCLUDE}/record/record_type.h ${BIRA_INCLUDE}/record/record_reader.h)

add_executable(biraread biraread.cpp ${HEADER})
target_compile_features(biraread PRIVATE cxx_std_17)
target_include_directories(biraread PRIVATE ${BIRA_INCLUDE})
target_link_libraries(biraread ${BIRA_LIBRARY})

install(TARGETS biraread DESTINATION bin)
