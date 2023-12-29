/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#pragma once
#ifndef __TL__thallium__enums_h__
#define __TL__thallium__enums_h__
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

/**
 * @brief Enumeration of comparison operators for depth, stencil, and sampler operations.
 *
 * Possible values of this enumeration represent comparison operators used in depth, stencil, and sampler operations.
 */
typedef enum TL_CompareOp_t {
    /// @brief Comparison always evaluates false.
    TL_COMPARE_OP_NEVER,
    /// @brief Comparison evaluates as `reference < test`.
    TL_COMPARE_OP_LESS,
    /// @brief Comparison evaluates as `reference == test`.
    TL_COMPARE_OP_EQUAL,
    /// @brief Comparison evaluates as `reference <= test`.
    TL_COMPARE_OP_LESS_OR_EQUAL,
    /// @brief Comparison evaluates as *reference* > *test*.
    TL_COMPARE_OP_GREATER,
    /// @brief Comparison evaluates as *reference* != *test*.
    TL_COMPARE_OP_NOT_EQUAL,
    /// @brief Comparison evaluates as *reference* >= *test*.
    TL_COMPARE_OP_GREATER_OR_EQUAL,
    /// @brief Comparison always evaluates true.
    TL_COMPARE_OP_ALWAYS,
} TL_CompareOp_t;

/**
 * @brief Enumeration of modes for polygon culling (discarding) at rasterization.
 *
 * This enumeration contains modes of polygon culling/discarding during the rasterization stage of a graphics pipeline.
 */
typedef enum TL_CullModeFlags_t {
    /// @brief Do not cull polygons.
    TL_CULL_MODE_NONE_BIT =             0x00,
    /// @brief Cull front-facing polygons.
    TL_CULL_MODE_FRONT_BIT =            0x01,
    /// @brief Cull back-facing polygons.
    TL_CULL_MODE_BACK_BIT =             0x02,
    /// @brief Cull all polygons.
    TL_CULL_MODE_FRONT_AND_BACK_BIT =   0x03,
} TL_CullModeFlags_t;

/**
 * @brief Enumeration containing severities to describe debug messages.
 *
 * This enumeration contains severities to describe debug messages.
 *
 * @sa @ref TL_Debugger_t
 * @sa @ref TL_DebuggerDescriptor_t
 */
typedef enum TL_DebugSeverityFlags_t {
    /// @brief Log messages
    TL_DEBUG_SEVERITY_VERBOSE_BIT = 0x10,
    /// @brief Notification messages
    TL_DEBUG_SEVERITY_NOTIF_BIT =   0x08,
    /// @brief Warning messages
    TL_DEBUG_SEVERITY_WARNING_BIT = 0x04,
    /// @brief Error messages
    TL_DEBUG_SEVERITY_ERROR_BIT =   0x02,
    /// @brief Fatal error messages
    TL_DEBUG_SEVERITY_FATAL_BIT =   0x01,

    /// @brief All message severities
    TL_DEBUG_SEVERITY_ALL_BIT =     0x1f,
} TL_DebugSeverityFlags_t;

/**
 * @brief Enumeration containing sources to describe debug messages.
 *
 * This enumeration contains message sources to describe debug messages.
 *
 * @sa @ref TL_Debugger_t
 * @sa @ref TL_DebuggerDescriptor_t
 */
typedef enum TL_DebugSourceFlags_t {
    /// @brief Messages reported by core Thallium functions
    TL_DEBUG_SOURCE_THALLIUM_BIT =  0x20,
    /// @brief Messages reported by Vulkan renderer systems
    TL_DEBUG_SOURCE_VULKAN_BIT =    0x10,

    /// @brief All message sources
    TL_DEBUG_SOURCE_ALL_BIT =       0x3f,
} TL_DebugSourceFlags_t;

/**
 * @brief Enumeration containing types of Thallium pipeline objects.
 *
 * This enumeration contains types of Thallium pipeline objects.
 *
 * @sa @ref TL_Pipeline_t
 * @sa @ref TL_PipelineDescriptor_t
 */
typedef enum TL_PipelineType_t {
    /// @brief Represent a graphics pipeline
    TL_PIPELINE_TYPE_GRAPHICS,
    /// @brief Represent a compute pipeline (currently unimplemented and reserved for future use)
    TL_PIPELINE_TYPE_COMPUTE,
    /// @brief Represent a ray tracing pipeline (currently unimplemented and reserved for future use)
    TL_PIPELINE_TYPE_RAY_TRACING,
} TL_PipelineType_t;

/**
 * @brief Enumeration of primitive rendering topologies.
 *
 * This enumeration contains supported primitive graphics topologies.
 *
 * @note Strip topology enumerators are indicated with the left-most bit set. When these options are used, a fixed vertex index value is used to
 * restart the assembly of primitives - this value is the maximum value for the type format used by the related index buffer.
 */
typedef enum TL_PrimitiveTopology_t {
    /// @brief A series of separate point primitives.
    TL_PRIMITIVE_TOPOLOGY_POINT_LIST =                  0x01,
    /// @brief A series of separate line primitives.
    TL_PRIMITIVE_TOPOLOGY_LINE_LIST =                   0x02,
    /// @brief A series of connected line primitives, with consecutive lines sharing a vertex.
    TL_PRIMITIVE_TOPOLOGY_LINE_STRIP =                  0x83,
    /// @brief A series of separate triangle primitives
    TL_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST =               0x04,
    /// @brief A series of connected triangle primitives, with consecutive triangles sharing an edge.
    TL_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP =              0x85,
    /// @brief A series of connected triangle primitives, with triangles sharing a common vertex.
    TL_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN =                0x06,
    /// @brief A series of separate line primitives with adjacency.
    TL_PRIMITIVE_TOPOLOGY_LINE_LIST_ADJACENCY =         0x07,
    /// @brief A series of connected line primitives with adjacency, with consecutive primitives sharing three vertices.
    TL_PRIMITIVE_TOPOLOGY_LINE_STRIP_ADJACENCY =        0x88,
    /// @brief A series of separate triangle primitives with adjacency.
    TL_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_ADJACENCY =     0x09,
    /// @brief A series of connected triangle primitives with adjacency, with consecutive primitives sharing an edge.
    TL_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_ADJACENCY =    0x8A,
    /// @brief Separate patch primitives.
    TL_PRIMITIVE_TOPOLOGY_PATCH_LIST =                  0x0B,
    /// @brief Null primitive topology.
    TL_PRIMITIVE_TOPOLOGY_NULL =                        0x00,
} TL_PrimitiveTopology_t;

/**
 * @brief Enumeration of polygon rasterization modes.
 *
 * This enumeration contains supported modes of polygon filling by the rasterizer.
 */
typedef enum TL_PolygonMode_t {
    /// @brief Draw filled polygons.
    TL_POLYGON_MODE_FILL,
    /// @brief Draw polygon edges (wireframe) only.
    TL_POLYGON_MODE_WIREFRAME,
    /// @brief Draw vertex points only.
    TL_POLYGON_MODE_POINT,
} TL_PolygonMode_t;

/**
 * @brief Enumeration containing graphics APIs for renderers.
 *
 * This enumeration contains all graphics APIs for renderers. Not all enumerations will necessarily be accepted by renderers on creation; ensure
 * that the appropriate module or modules were added to the build process when compiling the Thallium library.
 *
 * @sa @ref TL_Renderer_t
 * @sa @ref TL_RendererDescriptor_t
 */
typedef enum TL_RendererAPIFlags_t {
    /// @brief [Vulkan GPU API](https://www.vulkan.org/)
    TL_RENDERER_API_VULKAN_BIT =    0x10,

    /// @brief Null API. Not to be used in valid Thallium applications.
    TL_RENDERER_API_NULL_BIT =      0x00,
} TL_RendererAPIFlags_t;

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif
