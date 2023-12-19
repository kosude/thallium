/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#pragma once
#ifndef __TL__internal__core__swapchain_h__
#define __TL__internal__core__swapchain_h__
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

#include "thallium/core/swapchain.h"

typedef struct TL_Swapchain_t {
    /// @brief Internal API-aware swapchain system.
    void *swapchain_system;

    /// @brief Pointer to the parent renderer object.
    const TL_Renderer_t *renderer;
} TL_Swapchain_t;

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif