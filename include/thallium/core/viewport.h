/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#pragma once
#ifndef __TL__core__viewport_h__
#define __TL__core__viewport_h__
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

#include "thallium/platform.h"

/**
 * @brief A structure defining a viewport
 *
 * This structure contains floating-point data for viewport position (of the top-left corner), size, and depth range.
 */
typedef struct TL_Viewport_t {
    /// @brief X position of the viewport's top-left corner
    float x;
    /// @brief Y position of the viewport's top-left corner
    float y;
    /// @brief Width of the viewport
    float width;
    /// @brief Height of the viewport
    float height;
    /// @brief Minimum depth of the viewport
    float min_depth;
    /// @brief Maximum depth of the viewport
    float max_depth;
} TL_Viewport_t;

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

/**
 * @brief 2-dimensional offset (position) structure
 *
 * This 2-dimensional offset struct contains signed integral 2D position values.
 */
typedef struct TL_Offset2D_t {
    /// @brief X offset
    int32_t x;
    /// @brief Y offset
    int32_t y;
} TL_Offset2D_t;

/**
 * @brief 2-dimensional structure with unsigned extent and signed offset data.
 *
 * This 2-dimensional struct contains an unsigned extent and a signed offset.
 */
typedef struct TL_Rect2D_t {
    /// @brief Offset (position)
    TL_Offset2D_t offset;
    /// @brief Extent (size)
    TL_Extent2D_t extent;
} TL_Rect2D_t;

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif
