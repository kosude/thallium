set(SOURCES_CORE
    "state/state.c"

    "utils/io_colour.c"
    "utils/log.c"
    "utils/proc.c"

    "debug.c"
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
)

if (THALLIUM_BUILD_MODULE_VULKAN)
    target_compile_definitions(${PROJECT_NAME}_core PRIVATE "THALLIUM_VULKAN_INCL")
endif()


# API modules

if (THALLIUM_BUILD_MODULE_VULKAN)
    add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/vulkan")
endif()


# Interface library

add_library(${PROJECT_NAME} INTERFACE)
target_link_libraries(${PROJECT_NAME} INTERFACE ${THALLIUM_LIBRARIES})

message(STATUS "libthallium includes: ${THALLIUM_LIBRARIES}")
