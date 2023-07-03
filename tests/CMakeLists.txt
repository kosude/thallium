# compile dependencies
set(THALLIUM_TESTS_DEPS_DIR "${CMAKE_CURRENT_LIST_DIR}/deps")
set(THALLIUM_TEST_LINKS)
add_subdirectory("${THALLIUM_TESTS_DEPS_DIR}" "${PROJECT_BINARY_DIR}/tests/deps")

add_executable(hellotriangle "hello_triangle.cpp")
target_link_libraries(hellotriangle ${PROJECT_NAME} ${THALLIUM_TEST_LINKS})
target_include_directories(hellotriangle PRIVATE ${THALLIUM_TESTS_DEPS_DIR})
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(hellotriangle PUBLIC "_DEBUG")
endif()
