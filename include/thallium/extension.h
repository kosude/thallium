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
 * @brief An enumeration containing Vulkan-specific configuration flags
 *
 * This enumeration contains API-dependent configuration flags for Vulkan.
 *
 * @note One use of this property is to have a debug messenger internally appended onto the
 * instance in Vulkan renderers. To do this, simply specify any of the enumerators prefixed with
 * THALLIUM_VK_INSTANCE_DEBUG_MESSENGER_*. Note that this requires the debug utils extension to also
 * be defined in the renderer descriptor.
 */
typedef enum th_VulkanRendererExtensionFlag_t {
    /// @brief Create a debug messenger in Vulkan renderers that includes LOG messages.
    /* 00000001 */ THALLIUM_VK_INSTANCE_DEBUG_MESSENGER_SEV_LOG_BIT =           0x01,
    /// @brief Create a debug messenger in Vulkan renderers that includes INFO messages.
    /* 00000010 */ THALLIUM_VK_INSTANCE_DEBUG_MESSENGER_SEV_INFO_BIT =          0x02,
    /// @brief Create a debug messenger in Vulkan renderers that includes WARNING messages.
    /* 00000100 */ THALLIUM_VK_INSTANCE_DEBUG_MESSENGER_SEV_WARNING_BIT =       0x04,
    /// @brief Create a debug messenger in Vulkan renderers that includes ERROR messages.
    /* 00001000 */ THALLIUM_VK_INSTANCE_DEBUG_MESSENGER_SEV_ERROR_BIT =         0x08,
    /// @brief Create a debug messenger in Vulkan renderers that includes all debug message severities.
    /* 00001111 */ THALLIUM_VK_INSTANCE_DEBUG_MESSENGER_SEV_ALL_BIT =           0x0F,

    /// @brief Create a debug messenger in Vulkan renderers that includes GENERAL messages.
    /* 00010000 */ THALLIUM_VK_INSTANCE_DEBUG_MESSENGER_TYPE_GENERAL_BIT =      0x10,
    /// @brief Create a debug messenger in Vulkan renderers that includes VALIDATION messages.
    /* 00100000 */ THALLIUM_VK_INSTANCE_DEBUG_MESSENGER_TYPE_VALIDATION_BIT =   0x20,
    /// @brief Create a debug messenger in Vulkan renderers that includes PERFORMANCE messages.
    /* 01000000 */ THALLIUM_VK_INSTANCE_DEBUG_MESSENGER_TYPE_PERFORMANCE_BIT =  0x40,
    /// @brief Create a debug messenger in Vulkan renderers that includes DEVICE ADDRESS BINDING messages.
    // Note that this is only available with `VK_EXT_device_address_binding_report` specified.
    /* 10000000 */ THALLIUM_VK_INSTANCE_DEBUG_MESSENGER_TYPE_DEVICE_BIND_BIT =  0x80,
    /// @brief Create a debug messenger in Vulkan renderers that includes all debug message types.
    /* 11110000 */ THALLIUM_VK_INSTANCE_DEBUG_MESSENGER_TYPE_ALL_BIT =          0xF0,
} th_VulkanRendererExtensionFlag_t;

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

        /// @brief Configuration flags for Vulkan renderers
        int flags;
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
