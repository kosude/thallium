set(SOURCES
    "$<$<BOOL:${THALLIUM_WSI_COCOA}>:wsi/cocoa_mtbridge.m>"
    "$<$<BOOL:${THALLIUM_WSI_COCOA}>:wsi/cocoa_window_surface.c>"

    "$<$<BOOL:${THALLIUM_WSI_XCB}>:wsi/xcb_window_surface.c>"

    "$<$<BOOL:${THALLIUM_WSI_XLIB}>:wsi/xlib_window_surface.c>"

    "context.c"
    "debugger.c"
    "renderer.c"
    "swapchain.c"
)

# add path prefix to each filename in sources
list(TRANSFORM SOURCES PREPEND "${CMAKE_CURRENT_LIST_DIR}/")

set(THALLIUM_SOURCES ${THALLIUM_SOURCES} ${SOURCES} PARENT_SCOPE)
