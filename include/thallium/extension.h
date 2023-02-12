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
    THALLIUM_VK_INSTANCE_DEBUG_MESSENGER_SEV_LOG_BIT =           0x001,
    /// @brief Create a debug messenger in Vulkan renderers that includes INFO messages.
    THALLIUM_VK_INSTANCE_DEBUG_MESSENGER_SEV_INFO_BIT =          0x002,
    /// @brief Create a debug messenger in Vulkan renderers that includes WARNING messages.
    THALLIUM_VK_INSTANCE_DEBUG_MESSENGER_SEV_WARNING_BIT =       0x004,
    /// @brief Create a debug messenger in Vulkan renderers that includes ERROR messages.
    THALLIUM_VK_INSTANCE_DEBUG_MESSENGER_SEV_ERROR_BIT =         0x008,
    /// @brief Create a debug messenger in Vulkan renderers that includes all debug message severities.
    THALLIUM_VK_INSTANCE_DEBUG_MESSENGER_SEV_ALL_BIT =           0x00F,

    /// @brief Create a debug messenger in Vulkan renderers that includes GENERAL messages.
    THALLIUM_VK_INSTANCE_DEBUG_MESSENGER_TYPE_GENERAL_BIT =      0x010,
    /// @brief Create a debug messenger in Vulkan renderers that includes VALIDATION messages.
    THALLIUM_VK_INSTANCE_DEBUG_MESSENGER_TYPE_VALIDATION_BIT =   0x020,
    /// @brief Create a debug messenger in Vulkan renderers that includes PERFORMANCE messages.
    THALLIUM_VK_INSTANCE_DEBUG_MESSENGER_TYPE_PERFORMANCE_BIT =  0x040,
    /// @brief Create a debug messenger in Vulkan renderers that includes DEVICE ADDRESS BINDING messages.
    // Note that this is only available with `VK_EXT_device_address_binding_report` specified.
    THALLIUM_VK_INSTANCE_DEBUG_MESSENGER_TYPE_DEVICE_BIND_BIT =  0x080,
    /// @brief Create a debug messenger in Vulkan renderers that includes all debug message types.
    THALLIUM_VK_INSTANCE_DEBUG_MESSENGER_TYPE_ALL_BIT =          0x0F0,

    /// @brief Set instance debug messenger to print detailed information (usually not necessary).
    THALLIUM_VK_INSTANCE_DEBUG_MESSENGER_DETAILED =              0x100,
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
 * @param debugger Debugger to read configuration options from (or NULL for no debugging)
 * @return 0 if the descriptor was INVALID
 * @return 1 if the descriptor was VALID
 */
const int th_ValidateRendererExtensionDescriptor(
    const th_RendererExtensionDescriptor_t descriptor,
    const th_Debugger_t *debugger
);

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif // !_THALLIUM_PUB_EXTENSION_H
