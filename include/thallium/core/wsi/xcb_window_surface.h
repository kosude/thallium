/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#pragma once
#ifndef __TL__wsi__xcb_window_surface__
#define __TL__wsi__xcb_window_surface__
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

#include "thallium/fwd.h"

#include <xcb/xcb.h>

/**
 * @brief Create a Thallium window surface from an XCB window and connection.
 *
 * This function creates a window surface for Thallium functions, using an XCB window handle.
 *
 * @param connection XCB `xcb_connection_t` handle
 * @param window XCB `xcb_window_t` handle
 * @return Resulting window surface for use in Thallium functions.
 */
TL_WindowSurface_t *TL_CreateXCBWindowSurface(
    const xcb_connection_t *const connection,
    const xcb_window_t window
);

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif
