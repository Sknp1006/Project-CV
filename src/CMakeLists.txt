project(procv VERSION 1.0 LANGUAGES CXX)

option(BUILD_STATIC "Build as SHARE or STATIC library, default STATIC" ON)
message(STATUS "BUILD_STATIC:  ${BUILD_STATIC}")

# 3rdparty
find_package(OpenCV 4 REQUIRED)
list(APPEND 3RD_PARTY_INCLUDE_DIRS ${OpenCV_INCLUDE_DIRS})
list(APPEND 3RD_PARTY_LIBS ${OpenCV_LIBS})

# source files
file(GLOB_RECURSE CV_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

if (BUILD_STATIC STREQUAL "ON")
    message("------[Build as STATIC library]------")
    add_library(${PROJECT_NAME} STATIC ${CV_SRCS})
elseif(BUILD_STATIC STREQUAL "OFF")
    message("------[Build as SHARE library]------")
    add_library(${PROJECT_NAME} SHARED ${CV_SRCS})
else()
    message(FATAL_ERROR "BUILD_STATIC must be ON or OFF")
endif()

# include directories
target_include_directories(${PROJECT_NAME} PUBLIC ${3RD_PARTY_INCLUDE_DIRS})

# link libraries
target_link_libraries(${PROJECT_NAME} PUBLIC ${3RD_PARTY_LIBS})

# install target
install (TARGETS ${PROJECT_NAME} DESTINATION ${PROJECT_NAME}/lib)

# install header files
file(GLOB_RECURSE CV_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
install(FILES ${CV_HEADERS} DESTINATION ${PROJECT_NAME}/include)