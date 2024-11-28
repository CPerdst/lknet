cmake_minimum_required(VERSION 3.29)
project(lknet)

set(CMAKE_CXX_STANDARD 17)

# 设置输出路径
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# ---------------
# 编译 lklog 共享库
# ---------------

# 获取 lklog 源码
file(GLOB LKLOGSOURCE "lklog/src/source/*.cpp")
# 编译 lklog 共享库
add_library(lklog SHARED ${LKLOGSOURCE})
target_include_directories(lklog PRIVATE lklog/src/include)

# ---------------
# 编译 lknet 共享库
# ---------------

# 获取 lknet 源码
file(GLOB SOURCE "src/source/*.cpp")
# 编译 lknet 共享库
add_executable(lknet main.cpp ${SOURCE})
target_include_directories(lknet PRIVATE src/include lklog/src/include)
target_link_directories(lknet PRIVATE ${CMAKE_BINARY_DIR}/lib)
target_link_libraries(lknet PRIVATE pthread lklog)


