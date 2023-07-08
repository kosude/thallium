/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#pragma once
#ifndef __TL__internal__wsi__surface_platform_data_h__
#define __TL__internal__wsi__surface_platform_data_h__
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

#if defined(_THALLIUM_WSI_XCB)
#   include <xcb/xcb.h>
#endif
#if defined(_THALLIUM_WSI_XLIB)
#   include <X11/Xlib-xcb.h>
#endif

#if defined(_THALLIUM_WSI_XCB)

    // a struct which will be allocated for the platform_data member in XCB window surfaces
    typedef struct TL_WindowSurfacePlatformDataXCB_t {
        xcb_connection_t *connection;
        xcb_window_t window;
    } TL_WindowSurfacePlatformDataXCB_t;

#endif // _THALLIUM_WSI_XCB

#if defined(_THALLIUM_WSI_XLIB)

    // a struct which will be allocated for the platform_data member in XCB window surfaces
    typedef struct TL_WindowSurfacePlatformDataXlib_t {
        Display *display;
        Window window;
    } TL_WindowSurfacePlatformDataXlib_t;

#endif // _THALLIUM_WSI_XLIB

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif
