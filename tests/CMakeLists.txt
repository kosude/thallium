set(THALLIUM_TESTS_DEPS_DIR "${CMAKE_CURRENT_LIST_DIR}/deps")
set(THALLIUM_TEST_LINKS)

add_subdirectory("${THALLIUM_TESTS_DEPS_DIR}" "${PROJECT_BINARY_DIR}/tests/deps")

set(THALLIUM_TEST_COMMON_SRC
    "framework/object/Window.cpp"
    "framework/Test.cpp"
    "framework/Utils.cpp"
)

function(thallium_add_test TNAME TSRC)
    add_executable("${TNAME}" ${THALLIUM_TEST_COMMON_SRC} ${TSRC})
    target_link_libraries("${TNAME}" ${PROJECT_NAME} ${THALLIUM_TEST_LINKS} ${THALLIUM_LIBRARY_LINKS})
    target_include_directories("${TNAME}" PRIVATE ${THALLIUM_TESTS_DIR} ${THALLIUM_TESTS_DEPS_DIR} ${THALLIUM_DEPS_DIR})

    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_definitions(hellotriangle PUBLIC "_DEBUG")
    endif()
endfunction()

# tests defined below

thallium_add_test("hellotriangle" "bin/HelloTriangle.cpp")
