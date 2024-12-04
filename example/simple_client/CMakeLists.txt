cmake_minimum_required(VERSION 3.10)

add_executable(example_simple_client main.cpp)

target_include_directories(example_simple_client PRIVATE ../../src/include)

target_link_directories(example_simple_client PRIVATE ${CMAKE_BINARY_DIR}/lib)

target_link_libraries(example_simple_client PRIVATE lknet lklog)

message(STATUS "Initial example example_simple_client successful")