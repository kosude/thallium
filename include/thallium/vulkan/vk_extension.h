/*
 *   Copyright (c) 2023 Jack Bennett
 *   All rights reserved.
 *
 *   Please see the LICENCE file for more information.
 */

/**
 * @file vk_extension.h
 * @brief Vulkan-specific extension management
 *
 * @defgroup vk_extension Vulkan extensions and layers
 * @brief Handling Vulkan-specific extensions
 * @ingroup vulkan
 *
 * This group involves types and functions specific to handling Vulkan extensions and layers.
 */

#pragma once
#ifndef __thallium_vk_extension_h__
#define __thallium_vk_extension_h__
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

#include "thallium/fwd.h"
#include "thallium/fwdvk.h"


// ===========================================================================
//                           THALLIUM PUBLIC API
// ===========================================================================

/**
 * @ingroup vk_extension
 * @brief Get array of required layers for the given render system.
 *
 * This functions gets an array of required layers for Vulkan instances, from the information provided
 * through the given render system `render_system`.
 *
 * These layers are enabled on all Vulkan instances created with (internal function) thvk_CreateInstance().
 *
 * @param render_system Render system for querying
 * @param out_count location to which the amount of layers will be returned
 * @param out_layer_names NULL or the array to output layer names into.
 * @return @returnstatus
 */
int thvk_GetRequiredLayers(
    const thvk_RenderSystem_t *const render_system,
    unsigned int *const out_count,
    const char **out_layer_names
);

/**
 * @ingroup vk_extension
 * @brief Get array of required extensions for Vulkan instances.
 *
 * This functions populates an array of required extensions for Vulkan instances.
 *
 * These extensions are enabled on all Vulkan instances created with (internal function) thvk_CreateInstance().
 *
 * @param render_system Render system for querying
 * @param debug_utils_enabled Boolean - are debug utils enabled for the renderer?
 * @param out_count location to which the amount of extensions will be returned
 * @param out_extension_names NULL or the array to output extension names into.
 * @return @returnstatus
 */
int thvk_GetRequiredInstanceExtensions(
    const thvk_RenderSystem_t *const render_system,
    const int debug_utils_enabled,
    unsigned int *const out_count,
    const char **const out_extension_names
);

/**
 * @ingroup vk_extension
 * @brief Get array of required extensions for Vulkan devices.
 *
 * This functions populates an array of required extensions for Vulkan physical devices.
 *
 * The suitability of physical devices is partially based on these extensions. If a device does not support
 * all of the required extensions, it will not be considered suitable for the program.
 *
 * @param render_system Render system for querying
 * @param out_count location to which the amount of extensions will be returned
 * @param out_extension_names NULL or the array to output extension names into.
 * @return Array of extension names (NULL if error)
 */
int thvk_GetRequiredDeviceExtensions(
    const thvk_RenderSystem_t *const render_system,
    unsigned int *const out_count,
    const char **const out_extension_names
);

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif // !__thallium_vk_extension_h__
