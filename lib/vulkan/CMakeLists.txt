set(SOURCES_VULKAN
    "vk_extension.c"
    "vk_render_system.c"
)

find_package("Vulkan" REQUIRED)

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
)
target_link_libraries(${PROJECT_NAME}_vk ${PROJECT_NAME}_core ${Vulkan_LIBRARIES})

target_compile_definitions(${PROJECT_NAME}_vk PRIVATE ${THALLIUM_LIBRARY_DEFS})
