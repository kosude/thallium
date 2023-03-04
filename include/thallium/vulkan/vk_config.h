/*
 *   Copyright (c) 2023 Jack Bennett
 *   All rights reserved.
 *
 *   Please see the LICENCE file for more information.
 */

/**
 * @file vk_config.h
 * @brief Optional Vulkan renderer configuration
 *
 * @defgroup vk_config Optional Vulkan renderer configuration
 * @ingroup renderer
 * @brief Further configuring renderers for certain graphics APIs
 *
 * Descriptor structures for extra configuration of Vulkan [renderers](@ref renderer).
 */

#pragma once
#ifndef __thallium_vk_config_h__
#define __thallium_vk_config_h__
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

#include "thallium/core/version.h"
#include "thallium/fwdvk.h"

#include <volk/volk.h>


// ===========================================================================
//                           THALLIUM PUBLIC API
// ===========================================================================

/**
 * @ingroup vk_config
 * @brief Application information configuration struct for Vulkan configs
 *
 * This structure contains application info options for @ref thvk_VulkanRendererConfig_t structs.
 *
 * @sa @ref thvk_VulkanRendererConfig_t
 */
typedef struct thvk_VulkanRendererApplicationConfig_t {
    /// @brief The name of the application
    char *application_name;
    /// @brief The application's version
    th_Version_t application_version;
    /// @brief The name of the engine being used, if applicable
    char *engine_name;
    /// @brief The version of the engine being used, if applicable
    th_Version_t engine_version;
} thvk_VulkanRendererApplicationConfig_t;

/**
 * @ingroup vk_config
 * @brief Extra extension configuration struct for Vulkan configs
 *
 * This structure contains extra extension options for @ref thvk_VulkanRendererConfig_t structs.
 *
 * Note that layers and extensions specified in this struct will be appended to the list of
 * automatically required ones defined by Thallium (see @ref thvk_GetRequiredLayers(),
 * @ref thvk_GetRequiredInstanceExtensions(), etc).
 *
 * The `extension_names` array is to contain both instance and device extension names to be enabled.
 *
 * @sa @ref thvk_VulkanRendererConfig_t
 */
typedef struct thvk_VulkanRendererExtensionConfig_t {
    /// @brief An optional array of names of Vulkan layers to enable
    char **layer_names;
    /// @brief The amount of elements in `layer_names`
    int layer_count;

    /// @brief An optional array of names of extra extensions to enable on Vulkan instances/devices.
    char **extension_names;
    /// @brief The amount of elements in `extension_names`.
    int extension_count;
} thvk_VulkanRendererExtensionConfig_t;

/**
 * @ingroup vk_config
 * @brief Extra device configuration struct for Vulkan configs
 *
 * This structure contains extra device requirement options for @ref thvk_VulkanRendererConfig_t structs.
 *
 * @sa @ref thvk_VulkanRendererConfig_t
 */
typedef struct thvk_VulkanRendererDeviceConfig_t {
    /// @brief Optional features to require on Vulkan devices
    VkPhysicalDeviceFeatures features;

    /// @brief Set to 1 to require a graphics queue family
    int graphics_queue_exists;
    /// @brief Set to 1 to require a compute queue family
    int compute_queue_exists;
    /// @brief Set to 1 to require a transfer queue family
    int transfer_queue_exists;
} thvk_VulkanRendererDeviceConfig_t;

/**
 * @ingroup vk_config
 * @brief Configuration structure specifically for Vulkan renderers
 *
 * This structure provides exclusive configuration options specifically for Vulkan renderers.
 *
 * To apply options specified in this struct, pass it as a parameter in the @ref th_RendererDescriptor_t
 * descriptor which is passed to @ref th_CreateRenderer().
 *
 * @sa @ref th_CreateRenderer()
 * @sa @ref th_RendererDescriptor_t
 */
typedef struct thvk_VulkanRendererConfig_t {
    /// @brief Optional application info configuration
    thvk_VulkanRendererApplicationConfig_t application_config;
    /// @brief Optional extension configuration
    thvk_VulkanRendererExtensionConfig_t extension_config;
    /// @brief Optional device requirement configuration
    thvk_VulkanRendererDeviceConfig_t device_config;
} thvk_VulkanRendererConfig_t;

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif // !__thallium_vk_config_h__
