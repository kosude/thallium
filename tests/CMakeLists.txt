macro(ADD_TEST_PROJECT TEST_NAME TEST_FILES LIB_FILES)
    add_executable(${TEST_NAME} ${TEST_FILES})
    target_link_libraries(${TEST_NAME} ${LIB_FILES})
endmacro()

add_test_project(hellotriangle "hello_triangle.cpp" ${PROJECT_NAME})
