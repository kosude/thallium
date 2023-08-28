set(SOURCES
    "vk_context_block.c"
    "vk_device.c"
    "vk_instance.c"

    "vk_renderer_system.c"
    "vk_swapchain_system.c"
)

# add path prefix to each filename in sources
list(TRANSFORM SOURCES PREPEND "${CMAKE_CURRENT_LIST_DIR}/")

set(THALLIUM_SOURCES ${THALLIUM_SOURCES} ${SOURCES} PARENT_SCOPE)

set(THALLIUM_PUBLIC_INCLUDE_DIRS ${THALLIUM_PUBLIC_INCLUDE_DIRS}
    ${Vulkan_INCLUDE_DIRS}

    PARENT_SCOPE
)

set(THALLIUM_PUBLIC_LIBRARY_DEFS ${THALLIUM_PUBLIC_LIBRARY_DEFS}
    "_THALLIUM_VULKAN_INCL"

    PARENT_SCOPE
)

set(THALLIUM_MODULE_HEADER_CONTENTS
    "${THALLIUM_MODULE_HEADER_CONTENTS}\n#include \"thallium_vulkan.h\""

    PARENT_SCOPE
)
