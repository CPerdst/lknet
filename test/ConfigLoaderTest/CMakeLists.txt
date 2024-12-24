cmake_minimum_required(VERSION 3.10)

add_executable(config_loader_test main.cpp)

target_include_directories(config_loader_test PRIVATE ../../src/include)

target_link_directories(config_loader_test PRIVATE ${CMAKE_BINARY_DIR}/lib)

target_link_libraries(config_loader_test PRIVATE lknet lklog)

message(STATUS "Initial test ConfigLoaderTest successful")