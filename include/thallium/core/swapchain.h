/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#pragma once
#ifndef __TL__core__swapchain_h__
#define __TL__core__swapchain_h__
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

#include "thallium_decl/fwd.h"
#include "thallium/platform.h"

#include "thallium/core/viewport.h"

/**
 * @brief A structure to represent a graphical swapchain.
 *
 * This opaque structure provides a representation of a swapchain object.
 *
 * @sa @ref TL_SwapchainCreate()
 * @sa @ref TL_SwapchainDestroy()
 * @sa @ref TL_SwapchainDescriptor_t
 */
typedef struct TL_Swapchain_t TL_Swapchain_t;

/**
 * @brief A structure describing a swapchain object to be created.
 *
 * This structure describes a [swapchain object](@ref TL_Swapchain_t) to be created.
 */
typedef struct TL_SwapchainDescriptor_t {
    /// @brief Resolution of images in the swapchain
    TL_Extent2D_t resolution;

    /// @brief A Thallium [window surface object](@ref TL_WindowSurface_t).
    TL_WindowSurface_t *window_surface;

    /// @brief NULL or an optional descriptor for the API-specific swapchain system to be created within the swapchain. For example, to specify
    /// API-specific options to a Vulkan swapchain system, pass to this parameter a pointer to a @ref TLVK_SwapchainSystemDescriptor_t struct.
    void *swapchain_system_descriptor;
} TL_SwapchainDescriptor_t;

/**
 * @brief Create and return a new Thallium swapchain object for use with the given renderer.
 *
 * This function creates and returns a new Thallium swapchain object for use with the given Thallium renderer object `renderer`.
 *
 * Using the resultant swapchain with a different renderer will likely result in errors. Also note that swapchain objects are mostly **immutable**.
 * You cannot modify a swapchain for use with a different renderer, you must create a new one instead.
 *
 * @param renderer Renderer to create the swapchain for.
 * @param descriptor A swapchain descriptor struct.
 * @return The new swapchain
 *
 * @sa @ref TL_Swapchain_t
 */
TL_Swapchain_t *TL_SwapchainCreate(
    const TL_Renderer_t *const renderer,
    const TL_SwapchainDescriptor_t descriptor
);

/**
 * @brief Free the given swapchain object.
 *
 * This function frees the specified swapchain object.
 *
 * @param swapchain Pointer to the swapchain object to free.
 */
void TL_SwapchainDestroy(
    TL_Swapchain_t *const swapchain
);

/**
 * @brief Retrieve the extent of the given swapchain.
 *
 * This function returns the extent of the specified swapchain object.
 *
 * @param swapchain Swapchain pointer
 * @return Swapchain extent
 */
TL_Extent2D_t TL_SwapchainGetExtent(
    TL_Swapchain_t *const swapchain
);

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif
