project(test)

# gtest
include_directories(${CMAKE_SOURCE_DIR}/3rdparty/googletest/googletest/include)

# cv_core
add_executable(test_cv_core test_cv_core.cpp)
target_include_directories(test_cv_core PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_cv_core PRIVATE procv gtest gtest_main)
install(TARGETS test_cv_core DESTINATION ${PROJECT_NAME}/bin)

# test_img
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/imgs/test.jpg DESTINATION ${PROJECT_NAME}/bin)
