/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#pragma once
#ifndef __TL__core__renderer_h__
#define __TL__core__renderer_h__
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

#include "thallium/enums.h"
#include "thallium/fwd.h"
#include "thallium/platform.h"

#include "thallium/core/version.h"

/**
 * @brief A structure to hold flags for the features of a [renderer](@ref TL_Renderer_t) object.
 *
 * This structure allows the user to, for example, require desired Thallium-supported features from a renderer.
 *
 * Requiring the features that will be used throughout the program at renderer creation is required to access those features, as renderers are
 * immutable for the most part.
 *
 * @sa @ref TL_Renderer_t
 */
typedef struct TL_RendererFeatures_t {
    /// @brief The renderer can present images to a swapchain.
    bool presentation;
} TL_RendererFeatures_t;

/**
 * @brief A structure to represent a graphics API renderer.
 *
 * This opaque structure provides a representation of a certain graphics API.
 *
 * @sa @ref TL_CreateRenderer()
 * @sa @ref TL_RendererDestroy()
 * @sa @ref TL_RendererDescriptor_t
 */
typedef struct TL_Renderer_t TL_Renderer_t;

/**
 * @brief A structure describing a renderer object to be created.
 *
 * This structure describes a [renderer object](@ref TL_Renderer_t) to be created.
 */
typedef struct TL_RendererDescriptor_t {
    /// @brief Renderer API enumeration (**must not** be bitwise-OR'd - only one API is supported per renderer)
    TL_RendererAPIFlags_t api;
    /// @brief Renderer API version
    TL_Version_t api_version;

    /// @brief Renderer features to require
    TL_RendererFeatures_t requirements;

    /// @brief NULL or an optional descriptor for the API-specific render system to be created within the renderer. For example, to specify
    /// API-specific options to a Vulkan renderer, pass to this parameter a pointer to a @ref TLVK_RenderSystemDescriptor_t struct.
    void *render_system_descriptor;
} TL_RendererDescriptor_t;

/**
 * @brief Create `count` renderer(s) in the given Thallium context.
 *
 * This function creates `count` renderers into the [context object](@ref TL_Context_t) `context`. These renderers are configured using the array of
 * [descriptors](@ref TL_RendererDescriptor_t) given in `descriptors` (the length of which is also assumed to be `count`).
 *
 * The resultant array of renderers are allocated and output into `renderers`.
 *
 * The `count` parameter must be greater than 1. Ensure that both arrays are the same length. The index of each descriptor in the `descriptors` array
 * corresponds to the index of each renderer pointer in the `renderers` array.
 *
 * Neither `descriptors` or `renderers` can be NULL!
 *
 * @note This function can only be called **once** per context object! If invoked multiple times, the function will return `count`.
 *
 * @warning When loading the appropriate API(s), the highest version specified in the created renderer(s) is loaded. Therefore, if you are using
 * multiple renderers over the same graphics API, only functions from the highest specified version of that API is loaded.
 *
 * @param context The Thallium context to create the renderers in.
 * @param count Length of both `descriptors` and `renderers` arrays.
 * @param descriptors Array of descriptors for created renderers.
 * @param renderers Array of renderer pointers.
 * @param debugger NULL or a debugger for function debugging
 *
 * @return The amount of renderers that were successfully created
 */
uint32_t TL_RendererCreate(
    TL_Context_t *const context,
    const uint32_t count,
    const TL_RendererDescriptor_t *const descriptors,
    TL_Renderer_t **const *const renderers,
    const TL_Debugger_t *const debugger
);

/**
 * @brief Free the given renderer object.
 *
 * This function frees the specified renderer object.
 *
 * @param renderer Pointer to the renderer object to free.
 */
void TL_RendererDestroy(
    TL_Renderer_t *const renderer
);

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif
