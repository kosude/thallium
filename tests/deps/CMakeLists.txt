set(TEST_LINKS)


# If compiling tests, build and link GLFW

if (THALLIUM_BUILD_TESTS)
    # build GLFW
    set(GLFW_BUILD_TESTS OFF)
    set(GLFW_BUILD_DOCS OFF)
    set(GLFW_INSTALL OFF)
    add_subdirectory("${THALLIUM_TESTS_DEPS_DIR}/glfw" "${CMAKE_BINARY_DIR}/tests/deps/glfw")

    set(TEST_LINKS ${TEST_LINKS}
        glfw
    )
endif()


set(THALLIUM_TEST_LINKS ${THALLIUM_TEST_LINKS} ${TEST_LINKS} PARENT_SCOPE)
