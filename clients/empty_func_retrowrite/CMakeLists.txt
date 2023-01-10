cmake_minimum_required(VERSION 3.13.0)
project(empty_func_retrowrite LANGUAGES CXX)

# Instrumentation library
add_library(empty_func_retrowrite SHARED empty_func_retrowrite.cpp)
target_link_libraries(empty_func_retrowrite)
target_compile_options(empty_func_retrowrite PRIVATE ${WARNINGS})

install(TARGETS empty_func_retrowrite DESTINATION lib)
