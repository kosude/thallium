/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#pragma once
#ifndef __TL__internal__core__renderer_h__
#define __TL__internal__core__renderer_h__
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

#include "thallium/core/renderer.h"

typedef struct TL_Renderer_t {
    /// @brief Renderer API enumeration.
    TL_RendererAPIFlags_t api;

    /// @brief Pointer to the parent context object.
    const TL_Context_t *context;
    /// @brief NULL or attached debugger object.
    const TL_Debugger_t *debugger;

    /// @brief Internal API-aware render system.
    void *render_system;
} TL_Renderer_t;

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif
