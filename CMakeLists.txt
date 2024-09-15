cmake_minimum_required(VERSION 3.5)
project(Project-CV VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/install" CACHE STRING "Install path prefix, prepended onto install directories." FORCE)
message("CMAKE_INSTALL_PREFIX: " ${CMAKE_INSTALL_PREFIX})

set(3RD_PARTY_DIR "${CMAKE_SOURCE_DIR}/3rdparty")
set(CMAKE_PREFIX_PATH ${3RD_PARTY_DIR})

# 防止 BUILD_TYPE 与第三方库冲突
if (NOT DEFINED BUILD_TYPE)
    set(BUILD_TYPE "Debug")
endif()
if (BUILD_TYPE STREQUAL "Debug")
    add_compile_options(
        -O0     #编译优化等级0 不优化
        -Wall   #开启所有警告
        -g      #包含调试信息
        -fPIC   #位置无关代码
    )
    add_compile_definitions(_DEBUG)
    add_compile_definitions(SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE)
elseif(BUILD_TYPE STREQUAL "Release")
    add_compile_options(
        -O3     #编译优化等级3
        -fPIC   #位置无关代码
    )
    add_compile_definitions(SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_INFO)
else()
    message(FATAL_ERROR "BUILD_TYPE must be Debug or Release")
endif()
set(CMAKE_BUILD_TYPE ${BUILD_TYPE})
message("BUILD_TYPE: " ${BUILD_TYPE})

# 3rdparty
include(3rdparty/3rdparty.cmake)

# my project
add_subdirectory(src)
add_subdirectory(test)