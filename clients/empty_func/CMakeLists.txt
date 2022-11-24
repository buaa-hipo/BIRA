cmake_minimum_required(VERSION 3.13.0)
project(empty_func LANGUAGES CXX)

# Instrumentation library
add_library(empty_func SHARED empty_func.cpp)
target_link_libraries(empty_func)
target_compile_options(empty_func PRIVATE ${WARNINGS})

install(TARGETS empty_func DESTINATION lib)
