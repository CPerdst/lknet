cmake_minimum_required(VERSION 3.10)

add_executable(serialization_request main.cpp)

target_include_directories(serialization_request PRIVATE ../../src/include)

target_link_directories(serialization_request PRIVATE ${CMAKE_BINARY_DIR}/lib)

target_link_libraries(serialization_request PRIVATE lknet lklog)

message(STATUS "Initial example example_simple_client successful")