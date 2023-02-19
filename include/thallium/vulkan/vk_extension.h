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
#ifndef _THALLIUM_PUB_VULKAN_EXTENSION_H
#define _THALLIUM_PUB_VULKAN_EXTENSION_H
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

/// @cond DOCS_IGNORE

typedef struct th_Debugger_t th_Debugger_t;
typedef struct th_RendererConfigVulkan_t th_RendererConfigVulkan_t;

/// @endcond DOCS_IGNORE

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
    const th_RendererConfigVulkan_t *config
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
 * @return Array of extension names
 */
char **thvk_GetRequiredInstanceExtensions(
    unsigned int *out_count,
    const int debugUtilsEnabled,
    const th_Debugger_t *debugger
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
 * @return Array of layer names
 */
char **thvk_GetAvailableLayers(
    unsigned int *out_count,
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
 * @return Array of instance extension names
 */
char **thvk_GetAvailableInstanceExtensions(
    unsigned int *out_count,
    const char *const layerName,
    const th_Debugger_t *debugger
);

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif // !_THALLIUM_PUB_VULKAN_EXTENSION_H
