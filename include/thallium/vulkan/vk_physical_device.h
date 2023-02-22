/*
 *   Copyright (c) 2023 Jack Bennett
 *   All rights reserved.
 *
 *   Please see the LICENCE file for more information.
 */

/**
 * @file vk_physical_device.h
 * @brief Reading and picking from physical graphics devices based on Vulkan capabilities
 *
 * @defgroup vk_physical_device Evaluating Vulkan physical devices
 * @brief Functions for choosing the most optimal Vulkan physical device(s)
 * @ingroup vulkan
 *
 * This group contains functions involving Vulkan physical devices.
 */

#pragma once
#ifndef __thallium_vk_physical_device_h__
#define __thallium_vk_physical_device_h__
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

#include "thallium/fwdvk.h"

#include <vulkan/vulkan.h>


// ===========================================================================
//                           THALLIUM PUBLIC API
// ===========================================================================

/**
 * @ingroup vk_physical_device
 * @brief Structure to contain indices for queue families.
 *
 * This structure contains indices of queue families. It is returned by @ref thvk_GetQueueFamilyInfo().
 */
typedef struct thvk_QueueFamilyInfo_t {
    /// @brief Index of a queue family that supports graphics commands.
    int graphicsFamilyIndex;
    /// @brief Index of a queue family that supports presentation commands - applicable when surfaces are enabled.
    int presentFamilyIndex;
    /// @brief Index of a queue family that supports compute commands.
    int computeFamilyIndex;
    /// @brief Index of a queue family that supports transfer commands (will be a dedicated transfer queue family if available).
    int transferFamilyIndex;
} thvk_QueueFamilyInfo_t;

/**
 * @ingroup vk_physical_device
 * @brief Get array of physical devices available to the specified Vulkan instance.
 *
 * This function allocates and returns an array of physical devices available to Vulkan instance
 * `instance`.
 *
 * @note The returned array must be explicitly free'd.
 *
 * @param renderSystem Render system for querying and debugging
 * @param out_count Location into which the amount of physical devices will be returned
 * @return Heap-allocated array of Vulkan physical device structures.
 */
VkPhysicalDevice *thvk_GetAvailablePhysicalDevices(
    const thvk_RenderSystem_t *renderSystem,
    unsigned int *out_count
);

/**
 * @ingroup vk_physical_device
 * @brief Given an array of Vulkan physical devices, rank them and return the final sorted array.
 *
 * This function sorts the given array of Vulkan physical devices in order of suitability, and returns the
 * sorted array. **The item at index `0` is the most suitable, and the item at index `\*out_count - 1` is
 * the least suitable.**
 *
 * The length of the resultant array may be different from the length of `physicalDevices`. This occurs when
 * there are device(s) that do not support required extensions/features. Therefore, the length of the resultant
 * array is returned into `out_count`.
 *
 * **NULL will be returned if there are no suitable physical devices!**
 *
 * @note The returned array is allocated, and so must be explicitly free'd.
 *
 * @param physicalDevices Array of physical devices to copy and sort
 * @param physicalDeviceCount Amount of items in array `physicalDevices`.
 * @param renderSystem Render system for querying
 * @param out_count NULL or location into which the amount of suitable physical devices will be returned
 * @return Heap-allocated array of Vulkan physical device structures.
 */
VkPhysicalDevice *thvk_RankPhysicalDevices(
    const VkPhysicalDevice *physicalDevices,
    const unsigned int physicalDeviceCount,
    const thvk_RenderSystem_t *renderSystem,
    unsigned int *out_count
);

/**
 * @ingroup vk_physical_device
 * @brief Get the indices of all queue families within the given Vulkan physical device.
 *
 * This function gets the indices of all queue families within the given physical device, and returns them in
 * a @ref thvk_QueueFamilyInfo_t structure.
 *
 * @param physicalDevice Physical device to query.
 * @param debugger NULL or a pointer to a Thallium debugger
 * @return Struct containing queue family indices.
 */
const thvk_QueueFamilyInfo_t thvk_GetQueueFamilyInfo(
    const VkPhysicalDevice physicalDevice,
    const th_Debugger_t *debugger
);

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif // !__thallium_vk_physical_device_h__
