/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#pragma once
#ifndef __TL__vulkan__vk_render_system_h__
#define __TL__vulkan__vk_render_system_h__
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

#include "thallium/fwd.h"
#include "thallium/platform.h"

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.h>

/**
 * @brief A render system to hold Vulkan-specific rendering data.
 *
 * This structure holds per-renderer data for rendering with Vulkan.
 *
 * Note that this data **does not include a VkInstance**, as the instance is stored per-context rather than per-renderer. All Vulkan render sytems in
 * an application refer to the same instance, which is created when allocating and populating the context data.
 *
 * @sa @ref TLVK_CreateRenderSystem()
 */
typedef struct TLVK_RenderSystem_t {
    // TODO: set these properties in TLVK_CreateRenderSystem()...

    /// @brief The parent API-agnostic renderer object.
    const TL_Renderer_t *renderer;
    /// @brief The parent context object.
    const TL_Context_t *context;

    /// @brief Pointer (Vulkan handle) to the instance contained in `context`.
    VkInstance instance_ptr;
} TLVK_RenderSystem_t;

/**
 * @brief Descriptor struct to configure the creation of a Thallium render system for Vulkan.
 *
 * This descriptor structure provides options for the creation of Thallium render systems.
 */
typedef struct TLVK_RenderSystemDescriptor_t {
    uint32_t placeholder;
} TLVK_RenderSystemDescriptor_t;

/**
 * @brief Create and return a heap-allocated Vulkan render system.
 *
 * This function creates a new Vulkan render system and returns it. If there were any errors in
 * creation, NULL will be returned instead.
 *
 * @param renderer pointer to the parent renderer object
 * @param descriptor Thallium Vulkan render system descriptor
 * @return The new render system
 *
 * @sa @ref TLVK_RenderSystem_t
 * @sa @ref TLVK_DestroyRenderSystem()
 */
TLVK_RenderSystem_t *TLVK_CreateRenderSystem(
    const TL_Renderer_t *const renderer,
    const TLVK_RenderSystemDescriptor_t descriptor
);

/**
 * @brief Free the given Thallium Vulkan render system object.
 *
 * This function frees the specified render system object.
 *
 * @param render_system Pointer to the Thallium Vulkan render system to free.
 *
 * @sa @ref TLVK_RenderSystem_t
 * @sa @ref TLVK_CreateRenderSystem()
 */
void TLVK_DestroyRenderSystem(
    TLVK_RenderSystem_t *const render_system
);

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif
