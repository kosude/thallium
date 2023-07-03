/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#pragma once
#ifndef __TL__vulkan__vk_renderer_system_h__
#define __TL__vulkan__vk_renderer_system_h__
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

#include "thallium_decl/fwd.h"
#include "thallium_decl/fwdvk.h"
#include "thallium/platform.h"

/**
 * @brief A renderer system to hold Vulkan-specific rendering data.
 *
 * This structure holds per-renderer data for rendering with Vulkan.
 *
 * Note that this data **does not include a VkInstance**, as the instance is stored per-context rather than per-renderer. All Vulkan render sytems in
 * an application refer to the same instance, which is created when allocating and populating the context data.
 *
 * @sa @ref TLVK_RendererSystemCreate()
 */
typedef struct TLVK_RendererSystem_t TLVK_RendererSystem_t;

/**
 * @brief Descriptor struct to configure the creation of a Thallium renderer system for Vulkan.
 *
 * This descriptor structure provides options for the creation of Thallium renderer systems.
 */
typedef struct TLVK_RendererSystemDescriptor_t {
    /// @brief Descriptor for the renderer system [device manager object](@ref TLVK_DeviceManagerDescriptor_t)
    TLVK_DeviceManagerDescriptor_t *device_manager_descriptor;
} TLVK_RendererSystemDescriptor_t;

/**
 * @brief Create and return a heap-allocated Vulkan renderer system.
 *
 * This function creates a new Vulkan renderer system and returns it. If there were any errors in
 * creation, NULL will be returned instead.
 *
 * @param renderer pointer to the parent renderer object
 * @param descriptor Thallium Vulkan renderer system descriptor
 * @return The new renderer system
 *
 * @sa @ref TLVK_RendererSystem_t
 * @sa @ref TLVK_RendererSystemDestroy()
 */
TLVK_RendererSystem_t *TLVK_RendererSystemCreate(
    const TL_Renderer_t *const renderer,
    const TLVK_RendererSystemDescriptor_t descriptor
);

/**
 * @brief Free the given Thallium Vulkan renderer system object.
 *
 * This function frees the specified renderer system object.
 *
 * @param renderer_system Pointer to the Thallium Vulkan renderer system to free.
 *
 * @sa @ref TLVK_RendererSystem_t
 * @sa @ref TLVK_RendererSystemCreate()
 */
void TLVK_RendererSystemDestroy(
    TLVK_RendererSystem_t *const renderer_system
);

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif
