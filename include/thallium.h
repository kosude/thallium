/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#pragma once
#ifndef __TL__thallium_h__
#define __TL__thallium_h__
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

#include "thallium/enums.h"
#include "thallium/platform.h"

#include "thallium/core/context.h"
#include "thallium/core/debugger.h"
#include "thallium/core/renderer.h"
#include "thallium/core/version.h"

#include "thallium/core/wsi/window_surface.h"
#if defined(THALLIUM_WSI_XCB)
#   include "thallium/core/wsi/xcb_window_surface.h"
#endif
#if defined(THALLIUM_WSI_XLIB)
#   include "thallium/core/wsi/xlib_window_surface.h"
#endif

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif
