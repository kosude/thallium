set(THALLIUM_SOURCES)

set(THALLIUM_LIBRARY_LINKS)

set(THALLIUM_INCLUDE_DIRS
    "${THALLIUM_SRC_DIR}"
    "${PROJECT_BINARY_DIR}/generated"
    "${THALLIUM_DEPS_DIR}"
)
set(THALLIUM_PUBLIC_INCLUDE_DIRS
    "${THALLIUM_INCLUDE_DIR}"
)

set(THALLIUM_PUBLIC_LIBRARY_DEFS)

set(THALLIUM_MODULE_HEADER_CONTENTS)

# core library
add_subdirectory("${THALLIUM_SRC_DIR}/lib/core")
add_subdirectory("${THALLIUM_SRC_DIR}/utils")

# vulkan library
if (THALLIUM_BUILD_MODULE_VULKAN)
    add_subdirectory("${THALLIUM_SRC_DIR}/lib/vulkan")
endif()

# api_modules.h config
configure_file("${THALLIUM_CMAKE_DIR}/api_modules.h" "${PROJECT_BINARY_DIR}/generated/api_modules.h")

message(STATUS "thallium libraries define: ${THALLIUM_LIBRARY_DEFS}")
message(STATUS "thallium libraries publicly define: ${THALLIUM_PUBLIC_LIBRARY_DEFS}")

add_library(${PROJECT_NAME} ${THALLIUM_LIB_TYPE} ${THALLIUM_SOURCES})
target_link_libraries(${PROJECT_NAME} ${THALLIUM_LIBRARY_LINKS})
target_include_directories(${PROJECT_NAME} PUBLIC ${THALLIUM_PUBLIC_INCLUDE_DIRS} PRIVATE ${THALLIUM_INCLUDE_DIRS})
target_compile_definitions(${PROJECT_NAME} PUBLIC ${THALLIUM_PUBLIC_LIBRARY_DEFS} PRIVATE ${THALLIUM_LIBRARY_DEFS})
target_compile_options(${PROJECT_NAME} PRIVATE ${THALLIUM_COMPILE_OPTIONS})
