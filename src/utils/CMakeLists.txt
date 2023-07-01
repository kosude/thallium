set(THALLIUM_LIBRARY_LINKS ${THALLIUM_LIBRARY_LINKS}
    PARENT_SCOPE
)

set(SOURCES
    "io/log.c"
    "io/proc.c"
)

if (THALLIUM_BUILD_MODULE_VULKAN)
    set(SOURCES ${SOURCES}
        "vulkan/vk_pnext_append.c"
    )
endif()

# add path prefix to each filename in sources
list(TRANSFORM SOURCES PREPEND "${CMAKE_CURRENT_LIST_DIR}/")

set(THALLIUM_SOURCES ${THALLIUM_SOURCES} ${SOURCES} PARENT_SCOPE)
