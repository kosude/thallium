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
 * @brief Get array of required layers from the given renderer configuration.
 *
 * This functions gets an array of required layers for Vulkan instances, from the information provided
 * through the given `config` structure.
 *
 * These layers are enabled on all Vulkan instances created with (internal function) thvk_CreateInstance().
 *
 * The returned array is NOT heap-allocated!
 *
 * @param out_count NULL or location to which the amount of layers will be returned
 * @param config Pointer to a Vulkan renderer configuration
 * @return Array of layer names
 */
char **thvk_GetRequiredLayers(
    unsigned int *out_count,
    const th_RendererConfig_Vulkan_t *config
);

/**
 * @ingroup vk_extension
 * @brief Get array of available layers for Vulkan instances.
 *
 * This functions gets an array of available layers for Vulkan instances.
 *
 * @note The returned array is heap-allocated, and so must be explicitly freed.
 *
 * @param out_count NULL or location to which the amount of layers will be returned
 * @param debugger NULL or a pointer to a Thallium debugger
 * @return Array of layer names (NULL if error)
 */
char **thvk_GetAvailableLayers(
    unsigned int *out_count,
    const th_Debugger_t *debugger
);

/**
 * @ingroup vk_extension
 * @brief Get array of required extensions for Vulkan instances.
 *
 * This functions gets an array of required extensions for Vulkan instances.
 *
 * These extensions are enabled on all Vulkan instances created with (internal function) thvk_CreateInstance().
 *
 * @note The returned array is heap-allocated, and so must be explicitly freed.
 *
 * @param out_count NULL or location to which the amount of extensions will be returned
 * @param debugUtilsEnabled Boolean - are debug utils enabled for the renderer?
 * @param debugger NULL or a pointer to a Thallium debugger
 * @return Array of extension names (NULL if error)
 */
char **thvk_GetRequiredInstanceExtensions(
    unsigned int *out_count,
    const int debugUtilsEnabled,
    const th_Debugger_t *debugger
);

/**
 * @ingroup vk_extension
 * @brief Get array of available extensions for Vulkan instances.
 *
 * This functions gets an array of available extensions for Vulkan instances.
 *
 * If layerName is not NULL, the available extensions provided by that layer will be returned.
 * Otherwise, the available extensions provided by the Vulkan implementation will be returned.
 *
 * @note The returned array is heap-allocated, and so must be explicitly freed.
 *
 * @param out_count NULL or location to which the amount of extensions will be returned
 * @param layerName NULL or layer to check against
 * @param debugger NULL or a pointer to a Thallium debugger
 * @return Array of instance extension names (NULL if error)
 */
char **thvk_GetAvailableInstanceExtensions(
    unsigned int *out_count,
    const char *const layerName,
    const th_Debugger_t *debugger
);

/**
 * @ingroup vk_extension
 * @brief Get array of required extensions for Vulkan devices.
 *
 * This functions gets an array of required extensions for Vulkan physical devices.
 *
 * The suitability of physical devices is partially based on these extensions. If a device does not support
 * all of the required extensions, it will not be considered suitable for the program.
 *
 * @note The returned array is heap-allocated, and so must be explicitly freed.
 *
 * @param out_count NULL or location to which the amount of extensions will be returned
 * @param debugger NULL or a pointer to a Thallium debugger
 * @return Array of extension names (NULL if error)
 */
char **thvk_GetRequiredPhysicalDeviceExtensions(
    unsigned int *out_count,
    const th_Debugger_t *debugger
);

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif // !__thallium_vk_extension_h__
