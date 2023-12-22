/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#include "thallium/core/wsi/cocoa_window_surface.h"
#include "types/core/wsi/window_surface.h"

#include "utils/utils.h"

#include "cocoa_mtbridge.h"
#include "surface_platform_data.h"

#include <stdlib.h>

TL_WindowSurface_t *TL_WindowSurfaceCreateCocoa(void *const window, const TL_Debugger_t *const debugger) {
    TL_WindowSurface_t *surface = malloc(sizeof(TL_WindowSurface_t));
    if (!surface) {
        TL_Fatal(debugger, "MALLOC fault in call to TL_WindowSurfaceCreateCocoa");
        return NULL;
    }

    surface->wsi = TL_WSI_API_COCOA;

    // allocate space for platform data
    TL_WindowSurfacePlatformDataCocoa_t *platform_data = malloc(sizeof(TL_WindowSurfacePlatformDataCocoa_t));
    if (!platform_data) {
        TL_Fatal(debugger, "MALLOC fault in call to TL_WindowSurfaceCreateCocoa");
        return NULL;
    }

    id view = TL_NSWindowGetView(window);
    platform_data->mt_layer = TL_NSViewGetMetalLayer(view);

    surface->platform_data = (void *) platform_data;

    return surface;
}
