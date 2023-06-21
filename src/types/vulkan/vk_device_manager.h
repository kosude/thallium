/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#pragma once
#ifndef __TL__internal__vulkan__vk_device_manager_h__
#define __TL__internal__vulkan__vk_device_manager_h__
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

#include "thallium/vulkan/vk_device_manager.h"

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.h>

#include <cutils/carray/carray.h>

typedef struct TLVK_DeviceManager_t {
    /// @brief Vulkan physical device object used by the device manager.
    /// https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkPhysicalDevice.html
    VkPhysicalDevice vk_physical_device;
    /// @brief Handle to a Vulkan logical device.
    /// https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkDevice.html
    VkDevice vk_logical_device;

    /// @brief Array of enabled device extensions
    carray_t extensions;
    /// @brief Enabled device features.
    /// https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkPhysicalDeviceFeatures.html
    VkPhysicalDeviceFeatures vk_features;
} TLVK_DeviceManager_t;

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif
