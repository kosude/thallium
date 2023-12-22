/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#include "thallium/core/wsi/xlib_window_surface.h"
#include "types/core/wsi/window_surface.h"

#include "utils/utils.h"

#include "surface_platform_data.h"

#include <stdlib.h>

TL_WindowSurface_t *TL_WindowSurfaceCreateXlib(void *const display, const uint64_t window, const TL_Debugger_t *const debugger) {
    TL_WindowSurface_t *surface = malloc(sizeof(TL_WindowSurface_t));
    if (!surface) {
        TL_Fatal(debugger, "MALLOC fault in call to TL_WindowSurfaceCreateXlib");
        return NULL;
    }

    surface->wsi = TL_WSI_API_XLIB;

    // allocate platform data to hold xlib handles
    TL_WindowSurfacePlatformDataXlib_t *platform_data = malloc(sizeof(TL_WindowSurfacePlatformDataXlib_t));
    if (!platform_data) {
        TL_Fatal(debugger, "MALLOC fault in call to TL_WindowSurfaceCreateXlib");
        return NULL;
    }

    platform_data->window = (Window) window;
    platform_data->display = (Display *) display;

    surface->platform_data = (void *) platform_data;

    return surface;
}
