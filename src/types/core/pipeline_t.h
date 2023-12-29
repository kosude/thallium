/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#pragma once
#ifndef __TL__internal__core__pipeline_t_h__
#define __TL__internal__core__pipeline_t_h__
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

#include "thallium/core/pipeline.h"

typedef struct TL_Pipeline_t {
    /// @brief Internal API-aware pipeline system.
    void *pipeline_system;

    /// @brief Pointer to the parent renderer object.
    const TL_Renderer_t *renderer;
} TL_Pipeline_t;

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif
