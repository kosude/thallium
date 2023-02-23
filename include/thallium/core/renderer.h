/*
 *   Copyright (c) 2023 Jack Bennett
 *   All rights reserved.
 *
 *   Please see the LICENCE file for more information.
 */

/**
 * @file renderer.h
 * @brief Renderer structure(s) and related functions.
 *
 * @defgroup renderer Managing renderer objects
 * @brief Creating and managing renderer objects
 *
 * Renderer structure(s) and related functions.
 *
 * ### Explanation of terminology
 *
 * In Thallium, a **graphics API** such as OpenGL or Vulkan is represented by a structure called a
 * **renderer**. When building Thallium you need to make sure the appropriate **module** is compiled
 * for the desired renderer; otherwise, you will be attempting to use undefined functions.
 *
 * To summarise:
 *  - **API**: OpenGL, Vulkan, Metal, etc
 *  - **renderer**: user-created structure
 *  - **module**: section of the library source code to optionally be compiled
 */

#pragma once
#ifndef __thallium_renderer_h__
#define __thallium_renderer_h__
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

#include "thallium/fwd.h"
#include "thallium/core/version.h"


// ===========================================================================
//                           THALLIUM PUBLIC API
// ===========================================================================

/**
 * @ingroup renderer
 * @brief A structure to represent a graphics API.
 *
 * This opaque structure provides a representation of a certain graphics API.
 *
 * @sa th_CreateRenderer()
 */
typedef struct th_Renderer_t th_Renderer_t;

/**
 * @ingroup renderer
 * @brief A structure describing a renderer to be created.
 *
 * This structure describes a renderer to be created.
 *
 * Common practice is to set `rendererConfig` to NULL. However, it may be necessary to provide
 * [API-specific configuration](@ref renderer_config), which this parameter allows. See a Vulkan example below.
 *
 * @par Renderer config example
 * Say you are creating a Vulkan renderer, and want to enable additional layers: simply create a
 * `th_RendererConfig_Vulkan_t` structure with the relevant information, and pass a pointer to it via the `rendererConfig`
 * parameter.
 *
 * For more, see the @ref renderer_config page.
 */
typedef struct th_RendererDescriptor_t {
    /// @brief The name of the API being represented
    const char *api_name;
    /// @brief The version of the API being represented
    th_Version_t api_version;

    /// @brief Optional pointer to an API-specific configuration structure
    const void *renderer_config;
} th_RendererDescriptor_t;

/**
 * @ingroup renderer
 * @brief Create, populate, and return a new renderer struct.
 *
 * This function constructs and returns a pointer to a heap-allocated renderer struct.
 *
 * If `descriptor.apiName` is invalid, then NULL is returned. Note that this includes API modules that were not included
 * when Thallium was compiled. The validity of `descriptor.apiVersion` depends on the graphics API being used.
 *
 * For information on the `descriptor.rendererConfig` parameter, see the @ref th_RendererDescriptor_t documentation.
 *
 * @param descriptor creation description for the renderer
 * @param debugger Debugger to read configuration from when debugging (or NULL for no debugging)
 * @return Pointer to the new renderer object
 *
 * @sa @ref th_RendererDescriptor_t
 */
th_Renderer_t *th_CreateRenderer(
    const th_RendererDescriptor_t descriptor,
    th_Debugger_t *debugger
);

/**
 * @ingroup renderer
 * @brief Free the given renderer object.
 *
 * This function frees the specified renderer object. You should set the pointer to NULL
 * after calling this function, as you would normally when calling free().
 *
 * @param renderer Pointer to the renderer object to free.
 * @return @returnstatus
 *
 * @alwaysok
 */
const int th_DestroyRenderer(
    th_Renderer_t *renderer
);

/**
 * @ingroup renderer
 * @brief Get pointer to the render system within the given renderer.
 *
 * This function returns a pointer to the [render system](@ref vk_render_system) within the given
 * renderer.
 *
 * @param renderer Pointer to the renderer to evaluate
 * @return The render system within the renderer. This should not be NULL!
 */
const void *th_GetRendererRenderSystem(
    const th_Renderer_t *renderer
);

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif // !__thallium_renderer_h__
