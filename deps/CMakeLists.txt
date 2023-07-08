set(LIBRARY_LINKS)
set(INCLUDE_DIRS)


# Build and link cutils

set(CUTILS_CLIST OFF CACHE BOOL "Build clist library" FORCE)

add_subdirectory("${THALLIUM_DEPS_DIR}/cutils" "${CMAKE_BINARY_DIR}/deps/cutils")

set(LIBRARY_LINKS ${LIBRARY_LINKS}
    carray
    cio
)


# Link XCB if required

if (THALLIUM_WSI_XCB)
    find_package(XCB REQUIRED)

    set(LIBRARY_LINKS ${LIBRARY_LINKS} ${XCB_LIBRARIES})
    set(INCLUDE_DIRS ${INCLUDE_DIRS} ${XCB_INCLUDE_DIRS})
endif()


# Link Xlib if required

if (THALLIUM_WSI_XLIB)
    find_package(X11_XCB REQUIRED)

    set(LIBRARY_LINKS ${LIBRARY_LINKS} ${X11_XCB_LIBRARIES})
    set(INCLUDE_DIRS ${INCLUDE_DIRS} ${X11_XCB_INCLUDE_DIR})
endif()


# If compiling Vulkan module, build and link volk

if (THALLIUM_BUILD_MODULE_VULKAN)
    # require vulkan implementation
    find_package("Vulkan" REQUIRED)

    # build volk

    if (WIN32)
        set(VOLK_STATIC_DEFINES
            VK_USE_PLATFORM_WIN32_KHR)
    elseif (APPLE)
        set(VOLK_STATIC_DEFINES
            VK_USE_PLATFORM_METAL_EXT)
    elseif (UNIX)
        set(VOLK_STATIC_DEFINES
            VK_USE_PLATFORM_XCB_KHR
            VK_USE_PLATFORM_XLIB_KHR)
            # VK_USE_PLATFORM_WAYLAND_KHR
            # TODO: wayland support
    endif()

    add_subdirectory("${THALLIUM_DEPS_DIR}/volk" "${CMAKE_BINARY_DIR}/deps/volk")

    set(LIBRARY_LINKS ${LIBRARY_LINKS}
        volk
    )
endif()


set(THALLIUM_LIBRARY_LINKS ${THALLIUM_LIBRARY_LINKS} ${LIBRARY_LINKS} PARENT_SCOPE)
set(THALLIUM_INCLUDE_DIRS ${THALLIUM_INCLUDE_DIRS} ${INCLUDE_DIRS} PARENT_SCOPE)
