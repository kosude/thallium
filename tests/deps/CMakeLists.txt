set(TEST_LINKS)


# build and link GLFW

# build GLFW
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)
set(GLFW_INSTALL OFF)
add_subdirectory("${THALLIUM_TESTS_DEPS_DIR}/glfw" "${CMAKE_BINARY_DIR}/tests/deps/glfw")

set(TEST_LINKS ${TEST_LINKS}
    glfw
)


set(THALLIUM_TEST_LINKS ${THALLIUM_TEST_LINKS} ${TEST_LINKS} PARENT_SCOPE)
