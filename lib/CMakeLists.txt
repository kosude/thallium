set(SOURCES_CORE
    "utils/io_colour.c"
    "utils/log.c"
    "utils/primitive.c"
    "utils/proc.c"

    "debugger.c"
    "renderer.c"
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
        "${THALLIUM_DEPS_DIR}" # dependencies
)


# API modules

# variable to be substituted into generate/modules.h.
set(THALLIUM_MODULE_INCLUDES)

if (THALLIUM_BUILD_MODULE_VULKAN)
    add_subdirectory("${THALLIUM_LIB_DIR}/vulkan")

    set(THALLIUM_MODULE_INCLUDES "${THALLIUM_MODULE_INCLUDES}\n#include \"thallium_vulkan.h\"")
endif()

configure_file("${THALLIUM_CMAKE_DIR}/modules.h" "${PROJECT_BINARY_DIR}/generated/cmake_modules.h")

foreach(lib ${THALLIUM_LIBRARIES})
    target_compile_definitions(${lib}
        PUBLIC
            "THALLIUM_VERSION=\"${THALLIUM_VERSION}\""
            ${THALLIUM_PUBLIC_LIBRARY_DEFS}
        PRIVATE
            ${THALLIUM_LIBRARY_DEFS})

    target_compile_options(${lib} PRIVATE ${THALLIUM_COMPILE_OPTIONS})
endforeach()

message(STATUS "thallium libraries define: ${THALLIUM_LIBRARY_DEFS}")
message(STATUS "thallium libraries publicly define: ${THALLIUM_PUBLIC_LIBRARY_DEFS}")


# Interface library

add_library(${PROJECT_NAME} INTERFACE)
target_link_libraries(${PROJECT_NAME} INTERFACE ${THALLIUM_LIBRARIES})

message(STATUS "thallium includes: ${THALLIUM_LIBRARIES}")
