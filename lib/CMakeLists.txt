set(SOURCES_CORE
    "utils/io_colour.c"
    "utils/log.c"
    "utils/primitive.c"
    "utils/proc.c"

    "debugger.c"
    "renderer.c"
    "version.c"
)


# Core library

add_library(${PROJECT_NAME}_core ${LIB_BUILD_TYPE} ${SOURCES_CORE})

set(THALLIUM_LIBRARIES ${THALLIUM_LIBRARIES} ${PROJECT_NAME}_core)

target_include_directories(
    ${PROJECT_NAME}_core
    PUBLIC
        "${THALLIUM_INCLUDE_DIR}"
    PRIVATE
        "${THALLIUM_LIB_DIR}" # this directory for internal headers
        "${PROJECT_BINARY_DIR}/generated" # for generated resources and headers
)


# API modules

# variable to be substituted into generate/modules.h.
set(THALLIUM_MODULE_INCLUDES)

if (THALLIUM_BUILD_MODULE_VULKAN)
    add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/vulkan")

    set(THALLIUM_MODULE_INCLUDES "${THALLIUM_MODULE_INCLUDES}\n#include \"thallium_vulkan.h\"")
endif()

configure_file("${THALLIUM_CMAKE_DIR}/modules.h" "${PROJECT_BINARY_DIR}/generated/cmake_modules.h")

# add compilation definitions AFTER subdirectories so module flags are included
target_compile_definitions(${PROJECT_NAME}_core PRIVATE ${THALLIUM_LIBRARY_DEFS})

# Interface library

add_library(${PROJECT_NAME} INTERFACE)
target_link_libraries(${PROJECT_NAME} INTERFACE ${THALLIUM_LIBRARIES})

message(STATUS "libthallium includes: ${THALLIUM_LIBRARIES}")
