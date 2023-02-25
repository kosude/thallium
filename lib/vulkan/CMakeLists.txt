set(SOURCES_VULKAN
    "vk_extension.c"
    "vk_instance.c"
    "vk_physical_device.c"
    "vk_render_system.c"
)

find_package("Vulkan" REQUIRED)
set(THALLIUM_VULKAN_DEPENDENCIES ${THALLIUM_VULKAN_DEPENDENCIES} ${Vulkan_LIBRARIES} PARENT_SCOPE)

add_library(${PROJECT_NAME}_vk ${LIB_BUILD_TYPE} ${SOURCES_VULKAN})

set(THALLIUM_LIBRARIES ${THALLIUM_LIBRARIES} ${PROJECT_NAME}_vk PARENT_SCOPE)
set(THALLIUM_LIBRARY_DEFS ${THALLIUM_LIBRARY_DEFS} "THALLIUM_VULKAN_INCL" PARENT_SCOPE)

target_include_directories(
    ${PROJECT_NAME}_vk
    PUBLIC
        "${Vulkan_INCLUDE_DIRS}"
    PRIVATE
        "${THALLIUM_LIB_DIR}" # this directory for internal headers
        "${PROJECT_BINARY_DIR}/generated" # for generated resources and headers
        "${THALLIUM_DEPS_DIR}" # dependencies
)
target_link_libraries(${PROJECT_NAME}_vk ${PROJECT_NAME}_core ${THALLIUM_VULKAN_DEPENDENCIES})
