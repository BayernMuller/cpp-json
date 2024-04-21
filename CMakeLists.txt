cmake_minimum_required(VERSION 3.10)
project(cppjson)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(cppjson INTERFACE)
target_include_directories(cppjson INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/cppjson.h
        DESTINATION include/cppjson)

add_executable(example_main examples/main.cpp)
target_link_libraries(example_main PRIVATE cppjson)