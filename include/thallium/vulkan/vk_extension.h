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

/**
 * @ingroup vk_extension
 * @brief Validate the given instance extension
 *
 * Validate the given instance extension.
 *
 * If `layerName` is not NULL, that layer is used to validate the extension. Otherwise, the
 * extension is treated as global.
 *
 * @param extensionName The name of the instance extension to check
 * @param layerName NULL or the name of the layer to check the extension against
 * @return 0 if the instance extension was INVALID
 * @return 1 if the instance extension was VALID
 */
const int thvk_ValidateInstanceExtension(
    const char *const extensionName,
    const char *const layerName
);

/**
 * @ingroup vk_extension
 * @brief Validate the given layer
 *
 * Validate the given layer.
 *
 * @param layerName the name of the layer to validate
 * @return 0 if the layer was INVALID
 * @return 1 if the layer was VALID
 */
const int thvk_ValidateLayer(
    const char *const layerName
);

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif // !_THALLIUM_PUB_VULKAN_EXTENSION_H
