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

#include "thallium_decl/fwd.h"
#include "thallium/platform.h"

/**
 * @brief Create a Thallium window surface from an XCB window and connection.
 *
 * This function creates a window surface for Thallium functions, using an XCB window handle.
 *
 * @param connection XCB `xcb_connection_t *` handle (as a void ptr)
 * @param window XCB `xcb_window_t` handle (as an unsigned int)
 * @param debugger NULL or a debugger for function debugging
 * @return Resulting window surface for use in Thallium functions.
 */
TL_WindowSurface_t *TL_WindowSurfaceCreateXCB(
    void *const connection,
    const uint32_t window,
    const TL_Debugger_t *const debugger
);

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif
