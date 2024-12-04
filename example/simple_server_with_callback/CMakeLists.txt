cmake_minimum_required(VERSION 3.10)

add_executable(example_simple_server_with_callback main.cpp)

target_include_directories(example_simple_server_with_callback PRIVATE ../../src/include)

target_link_directories(example_simple_server_with_callback PRIVATE ${CMAKE_BINARY_DIR}/lib)

target_link_libraries(example_simple_server_with_callback PRIVATE lknet lklog)

message(STATUS "Initial example example_simple_server_with_callback successful")