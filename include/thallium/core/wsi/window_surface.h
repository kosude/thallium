/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#pragma once
#ifndef __TL__wsi__window_surface__
#define __TL__wsi__window_surface__
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

/**
 * @brief A portable Thallium structure to wrap around a platform-dependent, API-specific representation of a window.
 *
 * This structure wraps around a platform-dependent, API-specific representation of a window.
 */
typedef struct TL_WindowSurface_t TL_WindowSurface_t;

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif
