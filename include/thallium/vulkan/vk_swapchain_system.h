/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#pragma once
#ifndef __TL__vulkan__vk_swapchain_system_h__
#define __TL__vulkan__vk_swapchain_system_h__
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

#include "thallium_decl/fwd.h"
#include "thallium_decl/fwdvk.h"
#include "thallium/platform.h"

typedef struct VkSurfaceKHR_T *VkSurfaceKHR;

/**
 * @brief A renderer system to hold Vulkan-specific swapchain data.
 *
 * This structure holds data for a Vulkan swapchain.
 *
 * @sa @ref TLVK_SwapchainSystemCreate()
 * @sa @ref TLVK_SwapchainSystemDestroy()
 */
typedef struct TLVK_SwapchainSystem_t TLVK_SwapchainSystem_t;

/**
 * @brief Descriptor struct to configure the creation of a Thallium swapchain system for Vulkan.
 *
 * This descriptor structure provides options for the creation of Vulkan swapchains via Thallium Vulkan swapchain systems.
 */
typedef struct TLVK_SwapchainSystemDescriptor_t {
    /// @brief NULL or a Vulkan surface to use in swapchain creation.
    /// If this is NULL, a surface will be created based on the specified Thallium **window surface** (i.e. platform window handles). Otherwise, that
    /// window will be disregarded and this surface will be directly used instead.
    VkSurfaceKHR vk_surface;
} TLVK_SwapchainSystemDescriptor_t;

/**
 * @brief Create and return a heap-allocated swapchain system with Vulkan swapchain data.
 *
 * This function creates a new Vulkan swapchain system, including its
 * [Vulkan swapchain object](https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkSwapchainKHR.html) and returns it. If there were any
 * errors in creation, NULL will be returned instead.
 *
 * @note The parent renderer of the given Thallium renderer system `renderer_system` must have been created with the `presentation`
 * [feature](@ref TL_RendererFeatures_t).
 *
 * @note The `window_surface` parameter can be NULL **only if** a Vulkan surface is directly supplied to `descriptor.vk_surface` - otherwise, this
 * parameter **must** be a valid Thallium window surface object, in which case a new Vulkan surface will be created and stored by the swapchain.
 *
 * @param renderer_system a valid Thallium Vulkan renderer system object
 * @param resolution resolution of images in the swapchain
 * @param descriptor a Thallium Vulkan swapchain system descriptor
 * @param window_surface a Thallium window surface object <i>(can conditionally be NULL - see note above)</i>
 * @return The new swapchain system
 *
 * @sa @ref TLVK_SwapchainSystem_t
 * @sa @ref TLVK_SwapchainSystemDestroy()
 * @sa @ref TLVK_SwapchainSystemDescriptor_t
 */
TLVK_SwapchainSystem_t *TLVK_SwapchainSystemCreate(
    const TLVK_RendererSystem_t *const renderer_system,
    const TL_Extent2D_t resolution,
    const TLVK_SwapchainSystemDescriptor_t descriptor,
    const TL_WindowSurface_t *const window_surface
);

/**
 * @brief Free the given Thallium Vulkan swapchain system object.
 *
 * This function frees the specified swapchain system object.
 *
 * @param swapchain_system Pointer to the Thallium Vulkan swapchain system to free.
 *
 * @sa @ref TLVK_SwapchainSystem_t
 * @sa @ref TLVK_SwapchainSystemCreate()
 */
void TLVK_SwapchainSystemDestroy(
    TLVK_SwapchainSystem_t *const swapchain_system
);

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif
