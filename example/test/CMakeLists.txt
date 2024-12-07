cmake_minimum_required(VERSION 3.10)

add_executable(test main.cpp)

target_include_directories(test PRIVATE ../../src/include)

target_link_directories(test PRIVATE ${CMAKE_BINARY_DIR}/lib)

target_link_libraries(test PRIVATE lknet lklog)

message(STATUS "Initial example test successful")