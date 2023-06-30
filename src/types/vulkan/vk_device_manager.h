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

// internal struct to hold queue handles inside a device manager.
typedef struct TLVK_DeviceManagerQueues_t {
    /// @brief Amount of elements in `graphics_queues`.
    uint32_t graphics_queue_count;
    /// @brief NULL or an array of queues for graphics operations
    VkQueue *graphics_queues;

    /// @brief Amount of elements in `compute_queues`.
    uint32_t compute_queue_count;
    /// @brief NULL or an array of queues for compute operations
    VkQueue *compute_queues;

    /// @brief Amount of elements in `transfer_queues`.
    uint32_t transfer_queue_count;
    /// @brief NULL or an array of queues for memory transfer operations
    VkQueue *transfer_queues;

    /// @brief Amount of elements in `present_queues`.
    uint32_t present_queue_count;
    /// @brief NULL or an array of queues for presentation operations
    VkQueue *present_queues;
} TLVK_DeviceManagerQueues_t;

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

    /// @brief Vulkan queue handles:
    /// https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkQueue.html
    TLVK_DeviceManagerQueues_t vk_queues;
} TLVK_DeviceManager_t;

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif
