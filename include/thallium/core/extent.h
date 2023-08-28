/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#pragma once
#ifndef __TL__core__extent_h__
#define __TL__core__extent_h__
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

#include "thallium/platform.h"

/**
 * @brief 2-dimensional extent structure
 *
 * This 2-dimensional extent struct is used for unsigned integral 2D extents.
 */
typedef struct TL_Extent2D_t {
    /// @brief Extent width
    uint32_t width;
    /// @brief Extent height
    uint32_t height;
} TL_Extent2D_t;

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif
