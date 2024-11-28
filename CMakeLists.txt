cmake_minimum_required(VERSION 3.29)
project(lknet)

set(CMAKE_CXX_STANDARD 17)

file(GLOB SOURCE "src/source/*.cpp")

add_executable(lknet main.cpp ${SOURCE})
target_include_directories(lknet PRIVATE ./src/include)
target_link_libraries(lknet PRIVATE pthread)
