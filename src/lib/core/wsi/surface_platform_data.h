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

#if defined(_THALLIUM_WSI_COCOA)

#   if defined(__OBJC__)
#       import <Cocoa/Cocoa.h>
#   else
        typedef void *id;
#   endif

    // a struct which will be allocated for the platform_data member in Cocoa window surfaces
    typedef struct TL_WindowSurfacePlatformDataCocoa_t {
        id mt_layer; // CAMetalLayer
    } TL_WindowSurfacePlatformDataCocoa_t;

#endif

#if defined(_THALLIUM_WSI_XCB)

#   include <xcb/xcb.h>

    // a struct which will be allocated for the platform_data member in XCB window surfaces
    typedef struct TL_WindowSurfacePlatformDataXCB_t {
        xcb_connection_t *connection;
        xcb_window_t window;
    } TL_WindowSurfacePlatformDataXCB_t;

#endif

#if defined(_THALLIUM_WSI_XLIB)

#   include <X11/Xlib-xcb.h>

    // a struct which will be allocated for the platform_data member in XCB window surfaces
    typedef struct TL_WindowSurfacePlatformDataXlib_t {
        Display *display;
        Window window;
    } TL_WindowSurfacePlatformDataXlib_t;

#endif

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif
