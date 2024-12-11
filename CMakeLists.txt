cmake_minimum_required(VERSION 3.10)
project(lknet)

set(CMAKE_CXX_STANDARD 17)

# 设置输出路径
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib) # 静态库输出路径
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib) # 动态库输出路径
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin) # 二进制文件输出路径

# ---------------
# 定义全局宏
# ---------------

option(EXAMPLE_BUILD "Build example project" OFF) # 默认关闭

# ---------------
# 编译 lklog 共享库
# ---------------

# 引用 lklog 子模块
add_subdirectory(lklog)

# ------------
# 查找Boost库
# ------------

# 如果是 Windows 平台，需要设置对应的 Boost 库路径
if(WIN32)
    # 设置 Boost 配置路径，并查找 Boost 的 Boost_INCLUDE_DIR 和 Boost_LIBRARY_DIRS
    # 使用 GNU Compiler Collection 的 MingW 编译套件依赖于同样使用 MingW 编译的 Boost
    # 使用 MSVC 编译套件依赖于使用 MSVC 编译的 Boost
    set(Boost_DIR "D:\\boost-1.76.0-windows-x86-mingw\\lib\\cmake\\Boost-1.86.0")
endif ()

find_package(Boost REQUIRED)

if(Boost_FOUND)
    message(STATUS "Found boost library ${Boost_VERSION}")
else ()
    message(FATAL_ERROR "Boost library can not be found")
endif ()

# ---------------
# 编译 lknet 共享库
# ---------------

# 获取 lknet 源码
file(GLOB LKNET_SOURCE "src/source/*.cpp")
# 编译 lknet 共享库
add_library(lknet SHARED ${LKNET_SOURCE})

# 定义 Windows 编译链接库所需宏
if (WIN32)
    add_definitions(-D_WIN32_WINNT=0x0601)  # 目标 Windows 7
#    target_compile_definitions(lknet PRIVATE -D_WIN32_WINNT=0x0601)
    target_compile_definitions(lknet PRIVATE EXPORTING_DLL)
endif ()

# 包含头文件路径
target_include_directories(lknet PUBLIC src/include ${Boost_INCLUDE_DIR})

# 包含连接库路径
target_link_directories(lknet PUBLIC ${CMAKE_BINARY_DIR}/lib ${Boost_LIBRARY_DIRS})

# 查找不同平台下的 thread 库
find_package(Threads REQUIRED)

# 链接动态库
if(WIN32)
    target_link_libraries(lknet PUBLIC Threads::Threads lklog ws2_32 mswsock)
else ()
    target_link_libraries(lknet PUBLIC Threads::Threads lklog)
endif ()

# ----------------------
# 编译测试样例
# ----------------------

if (EXAMPLE_BUILD)
    add_subdirectory(example)
endif ()