project(test)

# gtest
include_directories(${CMAKE_SOURCE_DIR}/3rdparty/googletest/googletest/include)

# cv_core
add_executable(test_cv_core test_cv_core.cpp)
target_include_directories(test_cv_core PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_cv_core PRIVATE procv gtest gtest_main)
install(TARGETS test_cv_core DESTINATION ${PROJECT_NAME}/bin)

# cv_template
add_executable(test_cv_template test_cv_template.cpp)
target_include_directories(test_cv_template PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_cv_template PRIVATE procv gtest gtest_main)
install(TARGETS test_cv_template DESTINATION ${PROJECT_NAME}/bin)

# cv_face
add_executable(test_cv_face test_cv_face.cpp)
target_include_directories(test_cv_face PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_cv_face PRIVATE procv gtest gtest_main)
install(TARGETS test_cv_face DESTINATION ${PROJECT_NAME}/bin)
install(FILES ${CMAKE_SOURCE_DIR}/data/face_detection_yunet_2023mar.onnx DESTINATION ${PROJECT_NAME}/bin)

# cv_region
add_executable(test_cv_region test_cv_region.cpp)
target_include_directories(test_cv_region PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_cv_region PRIVATE procv gtest gtest_main)
install(TARGETS test_cv_region DESTINATION ${PROJECT_NAME}/bin)

# utils
add_executable(test_utils test_utils.cpp ${3RD_PARTY_DIR}/whereami/whereami.c)
target_include_directories(test_utils PRIVATE ${CMAKE_SOURCE_DIR}/utils ${3RD_PARTY_DIR}/whereami)
target_link_libraries(test_utils PRIVATE gtest gtest_main)
install(TARGETS test_utils DESTINATION ${PROJECT_NAME}/bin)

# cv_glcm
add_executable(test_cv_glcm test_cv_glcm.cpp)
target_include_directories(test_cv_glcm PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_cv_glcm PRIVATE procv gtest gtest_main)
install(TARGETS test_cv_glcm DESTINATION ${PROJECT_NAME}/bin)

# imgs
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/imgs/test.jpg DESTINATION ${PROJECT_NAME}/bin)


add_test(NAME test_cv_core COMMAND test_cv_core)
set_tests_properties(test_cv_core PROPERTIES WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}/bin)
add_test(NAME test_cv_template COMMAND test_cv_template)
set_tests_properties(test_cv_template PROPERTIES WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}/bin)
add_test(NAME test_cv_face COMMAND test_cv_face face_detection_yunet_2023mar.onnx test.jpg)
set_tests_properties(test_cv_face PROPERTIES WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}/bin)
add_test(NAME test_cv_region COMMAND test_cv_region)
set_tests_properties(test_cv_region PROPERTIES WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}/bin)
add_test(NAME test_utils COMMAND test_utils)
set_tests_properties(test_utils PROPERTIES WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}/bin)
add_test(NAME test_cv_glcm COMMAND test_cv_glcm)
set_tests_properties(test_cv_glcm PROPERTIES WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}/bin)
