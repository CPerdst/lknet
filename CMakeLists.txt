cmake_minimum_required(VERSION 3.10)
project(lknet)

set(CMAKE_CXX_STANDARD 17)

# 设置输出路径
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib) # 静态库输出路径
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib) # 动态库输出路径
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin) # 二进制文件输出路径

if (NOT CRT_BUILD_TYPE)
    set (CRT_BUILD_TYPE "static")
endif ()
# ---------------
# 禁止输出警告
# ---------------

if(MSVC)
    add_compile_options(/W0)
else()
    add_compile_options(/w)
endif()

# ---------------
# 定义全局宏
# ---------------

OPTION(EXAMPLE_BUILD "Build example project" OFF) # 默认关闭

OPTION(TEST_BUILD "Build test project" OFF) # 默认关闭

# ---------------
# 编译 lklog 共享库
# ---------------

# 引用 lklog 子模块
add_subdirectory(lklog)

# ------------
# 查找Boost库
# ------------

# 如果是 Windows 平台，需要设置对应的 Boost 库路径
if (WIN32)
    # 设置 Boost 配置路径，并查找 Boost 的 Boost_INCLUDE_DIR 和 Boost_LIBRARY_DIRS
    # 使用 GNU Compiler Collection 的 MingW 编译套件依赖于同样使用 MingW 编译的 Boost
    # 使用 MSVC 编译套件依赖于使用 MSVC 编译的 Boost
    if (USER_PRESET_BOOST_DIR)
        message(STATUS "Boost根路径： ${USER_PRESET_BOOST_DIR}")
        set(Boost_DIR ${USER_PRESET_BOOST_DIR})
    endif (USER_PRESET_BOOST_DIR)
endif (WIN32)

find_package(Boost REQUIRED)

if(Boost_FOUND)
    message(STATUS "Found boost library ${Boost_VERSION}")
    message(STATUS "Boost libraries: ${Boost_LIBRARIES}")
    message(STATUS "Boost include dirs: ${Boost_INCLUDE_DIRS}")
else ()
    message(FATAL_ERROR "Boost library can not be found")
endif ()

# ---------------
# 编译 lknet 共享库
# ---------------

# 获取 lknet 源码
file(GLOB LKNET_SOURCE "src/source/*.cpp")
# 编译 lknet 共享库
if (CRT_BUILD_TYPE STREQUAL "static")
    add_library(lknet STATIC ${LKNET_SOURCE})
    add_compile_definitions(STATIC_BUILD)
elseif (CRT_BUILD_TYPE STREQUAL "dynamic")
    add_library(lknet SHARED ${LKNET_SOURCE})
    add_compile_definitions(EXPORTING_DLL)
else ()
    message(FATAL_ERROR "Macro CRT_BUILD_TYPE: ${CRT_BUILD_TYPE} is invalid")
endif ()


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

if(TEST_BUILD)
    add_subdirectory(test)
endif ()