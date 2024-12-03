cmake_minimum_required(VERSION 3.29)
project(lknet)

set(CMAKE_CXX_STANDARD 17)

# 设置输出路径
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib) # 静态库输出路径
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib) # 动态库输出路径
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin) # 二进制文件输出路径

# ---------------
# 编译 lklog 共享库
# ---------------

# 通过 lklog 子库构建 liblklog.so 动态链接库
add_subdirectory(lklog)

# 获取 lklog 源码
#file(GLOB LKLOGSOURCE "lklog/src/source/*.cpp")
# 编译 lklog 共享库
#add_library(lklog SHARED ${LKLOGSOURCE})
#target_include_directories(lklog PRIVATE lklog/src/include)

# ------------
# 查找Boost库
# ------------

find_package(Boost REQUIRED COMPONENTS)

if(Boost_FOUND)
    message(STATUS "Found boost library ${Boost_VERSION}")
else ()
    message(FATAL_ERROR "Boost library can not be found")
endif ()

# ---------------
# 编译 lknet 共享库
# ---------------

# 获取 lknet 源码
file(GLOB LKNETSOURCE "src/source/*.cpp")
# 编译 lknet 共享库
add_library(lknet SHARED ${LKNETSOURCE})
target_include_directories(lknet PUBLIC src/include ${Boost_INCLUDE_DIR})
target_link_directories(lknet PUBLIC ${CMAKE_BINARY_DIR}/lib ${Boost_LIBRARY_DIRS})
target_link_libraries(lknet PUBLIC pthread lklog)


