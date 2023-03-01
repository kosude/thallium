set(SOURCES_VULKAN
    "vk_extension.c"
    "vk_instance.c"
    "vk_physical_device.c"
    "vk_render_system.c"
)

# build and link volk
if (WIN32)
    set(VOLK_STATIC_DEFINES VK_USE_PLATFORM_WIN32_KHR)
elseif (UNIX)
    set(VOLK_STATIC_DEFINES
        VK_USE_PLATFORM_XCB_KHR
        VK_USE_PLATFORM_XLIB_KHR)
        # VK_USE_PLATFORM_WAYLAND_KHR)
        # TODO: wayland support
elseif (APPLE)
    set(VOLK_STATIC_DEFINES VK_USE_PLATFORM_MACOS_MVK)
endif()
add_subdirectory("${THALLIUM_DEPS_DIR}/volk" "${CMAKE_BINARY_DIR}/deps")

# require vulkan impl
find_package("Vulkan" REQUIRED)

# build vulkan module
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
target_link_libraries(${PROJECT_NAME}_vk ${PROJECT_NAME}_core volk ${Vulkan_LIBRARIES})
