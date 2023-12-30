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
#include "thallium/platform.h"

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
 * @brief A descriptor struct for configuration of a graphics pipeline's rasterization stage ('rasterizer').
 *
 * This structure describes a pipeline's rasterization stage ('rasterizer').
 */
typedef struct TL_PipelineRasterizerDescriptor_t {
    /// @brief If true, primitives will practically not be clipped to the near and far planes of the frustum.
    bool depth_clamp;
    /// @brief If true, primitives will be discarded immediately before rasterization, ending the pipeline after the vertex processing stage.
    bool rasterizer_discard;

    /// @brief Polygon render mode - `TL_POLYGON_MODE_FILL` is assumed by default.
    TL_PolygonMode_t polygon_mode;
    /// @brief Bitmask of modes for polygon discarding a.k.a. culling. By default, no polygons are culled (`TL_CULL_MODE_NONE_BIT`).
    TL_CullModeFlags_t cull_modes;
    /// @brief If true, then clockwise-winding polygons will be assumed to be front-facing. Otherwise, counter-clockwise polygons are front-facing.
    bool clockwise_front_face;

    /// @brief If true, fragment depth values will be biased according to other `depth_bias_*` parameters.
    bool depth_bias;
    /// @brief A constant value to be added to the depth of each fragment.
    float depth_bias_constant_factor;
    /// @brief A factor applied to a fragment's slope in depth bias calculations.
    float depth_bias_slope_factor;
    /// @brief The maximum (or minimum) depth bias of a fragment.
    float depth_bias_clamp;

    /// @brief The width of all line primitives.
    float line_width;
} TL_PipelineRasterizerDescriptor_t;

/**
 * @brief A descriptor struct for configuration of a graphics pipeline's depth testing stage.
 *
 * This structure describes a graphics pipeline's depth testing stage.
 */
typedef struct TL_PipelineDepthTestDescriptor_t {
    /// @brief If true, the depth test is enabled.
    bool test_enabled;
    /// @brief If true, then writing to the depth buffer is enabled.
    bool write_enabled;

    /// @brief Depth test comparison function - `TL_COMPARE_OP_NEVER` is the default value.
    TL_CompareOp_t compare_op;
} TL_PipelineDepthTestDescriptor_t;

/**
 * @brief A structure describing a pipeline object to be created.
 *
 * This structure describes a [pipeline object](@ref TL_Pipeline_t) to be created.
 */
typedef struct TL_PipelineDescriptor_t {
    /// @brief The type of pipeline object to create.
    TL_PipelineType_t type;

    /// @brief The topology to assume for assembly of primitives. If NULL, then `TL_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST` is used by default.
    /// This value is ignored in compute and ray tracing pipelines.
    TL_PrimitiveTopology_t primitive_topology;
    /// @brief A [descriptor](@ref TL_PipelineRasterizerDescriptor_t) of the rasterization stage of the pipeline.
    /// This value is ignored in compute and ray tracing pipelines.
    TL_PipelineRasterizerDescriptor_t rasterizer;
    /// @brief A [descriptor](@ref TL_PipelineDepthTestDescriptor_t) of the depth-testing stage of the pipeline.
    /// This value is ignored in compute and ray tracing pipelines.
    TL_PipelineDepthTestDescriptor_t depth_test;

    /// @brief The amount of viewports in the `viewports` array; ignored if `viewports` is NULL.
    /// This value is ignored in compute and ray tracing pipelines.
    uint32_t viewport_count;
    /// @brief NULL or an array of viewports - if NULL, any viewports must be set dynamically instead.
    /// This value is ignored in compute and ray tracing pipelines.
    TL_Viewport_t *viewports;

    /// @brief The amount of viewports in the `viewports` array; ignored if `viewports` is NULL.
    /// This value is ignored in compute and ray tracing pipelines.
    uint32_t scissor_count;
    /// @brief NULL or an array of scissor rectangles - if NULL, any scissors must be set dynamically instead.
    /// This value is ignored in compute and ray tracing pipelines.
    TL_Rect2D_t *scissors;
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
