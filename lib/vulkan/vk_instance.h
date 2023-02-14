/*
 *   Copyright (c) 2023 Jack Bennett
 *   All rights reserved.
 *
 *   Please see the LICENCE file for more information.
 */

/**
 * @file vk_instance.h
 * @brief Some functions for creating and handling Vulkan instances.
 *
 * @defgroup vk_instance Vulkan instance functions
 * @brief Internally used functions for managing Vulkan instances.
 * @ingroup vulkan
 *
 * Some functions for creating and handling Vulkan instances. These are frequently used when managing
 * Vulkan render systems.
 */

#pragma once
#ifndef _THALLIUM_VULKAN_INSTANCE_H
#define _THALLIUM_VULKAN_INSTANCE_H
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

#include <vulkan/vulkan.h>
#include "thallium/vulkan/vk_render_system.h"

/**
 * @ingroup vk_instance
 * @brief Callback function used for instance debug messengers.
 *
 * This Vulkan debug callback function is used when Vulkan instances have debug utils enabled.
 *
 * @param severity Debug message severity
 * @param type Debug message type
 * @param callbackData Extra data passed to callback by the Vulkan API
 * @param userData Data passed to callback by the user
 * @return Always VK_FALSE
 */
extern const PFN_vkDebugUtilsMessengerCallbackEXT thvk_InstanceDebugMessengerCallback;

/**
 * @ingroup vk_instance
 * @brief Create a Vulkan instance.
 *
 * This function creates a Vulkan instance into the location `instance`.
 *
 * @param instance Pointer to the location into which the Vulkan instance will be returned.
 * @param descriptor Description of the render system that the instance is intended to be a part of.
 * @return @returnstatus
 */
const int thvk_CreateInstance(
    VkInstance *instance,
    const thvk_RenderSystemDescriptor_t descriptor
);

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif // !_THALLIUM_VULKAN_INSTANCE_H
