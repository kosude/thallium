/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#pragma once
#ifndef __TL__wsi__xlib_window_surface__
#define __TL__wsi__xlib_window_surface__
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

#include "thallium_decl/fwd.h"
#include "thallium/platform.h"

/**
 * @brief Create a Thallium window surface from an Xlib (X11) display and window handle.
 *
 * This function creates a window surface for Thallium functions, using an X11/Xlib window handle.
 *
 * @param display Xlib `Display` connection to the X server (as a void ptr)
 * @param window Xlib `Window` handle (as an unsigned long)
 * @return Resulting window surface for use in Thallium functions.
 */
TL_WindowSurface_t *TL_WindowSurfaceCreateXlib(
    const void *const display,
    const uint64_t window
);

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif
