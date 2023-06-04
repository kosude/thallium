/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#pragma once
#ifndef __TL__internal__core__context_h__
#define __TL__internal__core__context_h__
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

#include "thallium/core/context.h"

#include "thallium/platform.h"

typedef struct TL_Context_t {
    /// @brief Has TL_CreateContextAPIObjects() been called on this context object?
    bool api_objects_init;
    /// @brief Has TL_CreateRenderers() been called on this context object?
    bool renderers_init;

    /// @brief Memory offset from the .data member to the Vulkan-specific block of data
    /// @note If this value is SIZE_MAX, then the Vulkan block has not been initialised
    size_t vulkan_offset;

    /// @brief The size of the memory allocated at the .data pointer
    size_t data_size;
    /// @brief Context API-aware data
    void *data;
} TL_Context_t;

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif
