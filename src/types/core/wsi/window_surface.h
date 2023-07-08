/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#pragma once
#ifndef __TL__internal__wsi__window_surface__
#define __TL__internal__wsi__window_surface__
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

#include "thallium/core/wsi/window_surface.h"

/**
 * @brief An enumerator containing the window system APIs supported by Thallium window surfaces.
 *
 * This enumerator contains the window system APIs supported by Thallium window surfaces.
 */
typedef enum TL_WSI_API_t {
    /// @brief XCB window system
    TL_WSI_API_XCB,
    /// @brief X11 (Xlib) window system
    TL_WSI_API_XLIB,
} TL_WSI_API_t;

typedef struct TL_WindowSurface_t {
    /// @brief The window system used by the window surface
    /// This enum is helpful to know how to interpret the data stored by this struct.
    TL_WSI_API_t wsi;

    /// @brief The window platform data - i.e. the native handle(s) for the window.
    /// E.g., this can point to a struct containing handles of types xcb_connection_t and xcb_window_t.
    void *platform_data;
} TL_WindowSurface_t;

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif
