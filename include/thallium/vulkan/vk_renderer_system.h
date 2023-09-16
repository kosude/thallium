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
#include "thallium_decl/enumsvk.h"
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
 * @sa @ref TLVK_RendererSystemDestroy()
 */
typedef struct TLVK_RendererSystem_t TLVK_RendererSystem_t;

/**
 * @brief Descriptor struct to configure the creation of a Thallium renderer system for Vulkan.
 *
 * This descriptor structure provides options for the creation of Thallium renderer systems.
 */
typedef struct TLVK_RendererSystemDescriptor_t {
    /// @brief A @ref TLVK_PhysicalDeviceSelectionMode_t enum indicating the mode of selection for the Vulkan physical device used by the renderer
    /// system
    TLVK_PhysicalDeviceSelectionMode_t physical_device_mode;
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
 * @sa @ref TLVK_RendererSystemDescriptor_t
 */
TLVK_RendererSystem_t *TLVK_RendererSystemCreate(
    TL_Renderer_t *const renderer,
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
