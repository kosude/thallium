/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#pragma once
#ifndef __TL__core__pipeline_h__
#define __TL__core__pipeline_h__
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

#include "thallium_decl/enums.h"
#include "thallium_decl/fwd.h"

/**
 * @brief A structure to represent a pipeline object.
 *
 * This opaque structure represents a graphics, compute, or ray tracing pipeline object.
 *
 * @sa @ref TL_PipelineCreate()
 * @sa @ref TL_PipelineDestroy()
 * @sa @ref TL_PipelineDescriptor_t
 */
typedef struct TL_Pipeline_t TL_Pipeline_t;

/**
 * @brief A structure describing a pipeline object to be created.
 *
 * This structure describes a [pipeline object](@ref TL_Pipeline_t) to be created.
 */
typedef struct TL_PipelineDescriptor_t {
    /// @brief The type of pipeline object to create
    TL_PipelineType_t type;
} TL_PipelineDescriptor_t;

/**
 * @brief Create and return a handle to a new Thallium pipeline object under the given renderer.
 *
 * This function creates and returns a new Thallium pipeline object for use with the specified renderer.
 *
 * @param renderer Renderer to create the pipeline for.
 * @param descriptor A pipeline descriptor struct.
 * @return The new pipeline
 *
 * @sa @ref TL_Pipeline_t
 */
TL_Pipeline_t *TL_PipelineCreate(
    const TL_Renderer_t *const renderer,
    const TL_PipelineDescriptor_t descriptor
);

/**
 * @brief Free the given pipeline object.
 *
 * This function frees the specified pipeline object.
 *
 * @param pipeline Pointer to the pipeline object to free.
 */
void TL_PipelineDestroy(
    TL_Pipeline_t *const pipeline
);

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif
