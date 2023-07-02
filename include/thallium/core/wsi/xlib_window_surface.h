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

#include "thallium/fwd.h"

#include <X11/Xlib-xcb.h>

/**
 * @brief Create a Thallium window surface from an Xlib (X11) display and window handle.
 *
 * This function creates a window surface for Thallium functions, using an X11/Xlib window handle.
 *
 * @param display Xlib `Display` connection to the X server
 * @param window Xlib `Window` handle
 * @return Resulting window surface for use in Thallium functions.
 */
TL_WindowSurface_t *TL_CreateXlibWindowSurface(
    const Display *display,
    const Window window
);

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif
