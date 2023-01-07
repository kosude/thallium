/*
 *   Copyright (c) 2023 Jack Bennett
 *   All rights reserved.
 *
 *   Please see the LICENCE file for more information.
 */

/**
 * @file extension.h
 * @brief Structures related to extending renderers
 *
 * @defgroup extension Extending graphics APIs and renderers
 * @brief Managing graphics API extensions
 *
 * Members of this group allow you to extend certain graphics APIs as you need.
 */

#pragma once
#ifndef _THALLIUM_PUB_EXTENSION_H
#define _THALLIUM_PUB_EXTENSION_H
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

/**
 * @ingroup extension
 * @brief A struct to describe extensions to each graphics API
 *
 * This struct describes extensions to each graphics API.
 *
 * Because extending the renderer is inherently API-dependent, this struct is split into
 * smaller, API-specific structures that contain the different equivalents for each API.
 *
 * When defining the extensions you need in this structure, you should also consider defining
 * the equivalents to those extensions in all other APIs you wish to support.
 */
typedef struct th_RendererExtensionDescriptor_t {
    /// @brief Description to extend Vulkan renderers
    struct {
        /// @brief An array of names of Vulkan instance extensions to enable,.
        const char **extensionNames;
        /// @brief The amount of elements in `extensionNames`.
        int extensionCount;

        /// @brief An array of names of Vulkan layers to enable
        const char **layerNames;
        /// @brief The amount of elements in `layerNames`.
        int layerCount;
    } vulkan;
} th_RendererExtensionDescriptor_t;

/**
 * @ingroup extension
 * @brief Validate the relevant contents of the given renderer extension descriptor
 *
 * This function validates the contents of the given renderer extension descriptor that are
 * relevant to any graphics API modules that were compiled.
 *
 * The descriptor structure is not changed.
 *
 * @note Extensions for API modules that weren't compiled are not considered; for example, any
 * Vulkan layers specified in the descriptor will only be validated if the Vulkan module was
 * compiled. Otherwise, they will be ignored.
 *
 * @param descriptor The extension descriptor to check
 * @return 0 if the descriptor was INVALID
 * @return 1 if the descriptor was VALID
 */
const int th_ValidateRendererExtensionDescriptor(
    const th_RendererExtensionDescriptor_t descriptor
);

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif // !_THALLIUM_PUB_EXTENSION_H
