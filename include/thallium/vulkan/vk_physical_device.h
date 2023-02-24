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
    int graphics_family_index;
    /// @brief Index of a queue family that supports presentation commands - applicable when surfaces are enabled.
    int present_family_index;
    /// @brief Index of a queue family that supports compute commands.
    int compute_family_index;
    /// @brief Index of a queue family that supports transfer commands (will be a dedicated transfer queue family if available).
    int transfer_family_index;
} thvk_QueueFamilyInfo_t;

/**
 * @ingroup vk_physical_device
 * @brief Given an array of Vulkan physical devices, rank them and return the final sorted array (into `out_physical_devices`).
 *
 * This function sorts the given array of Vulkan physical devices in order of suitability, and returns the
 * sorted array. **The item at index `0` is the most suitable, and the item at index `\*out_count - 1` is
 * the least suitable.**
 *
 * The length of the resultant array may be different from the length of `physical_devices`. This occurs when
 * there are device(s) that do not support required extensions/features. Therefore, the length of the resultant
 * array is returned into `out_count`.
 *
 * The sorted array, that returned into `out_physical_devices`, contains *pointers* to the devices in `physical_devices`. The
 * structures are not copied.
 *
 * **0 will be returned if there are no suitable physical devices!**
 *
 * @note If `out_physical_devices` is NULL, no debug output will be reported to avoid duplicated debug output (as it is usually
 * called twice to get count and data).
 *
 * @param render_system Render system for querying
 * @param physical_devices Array of physical devices to copy and sort
 * @param physical_device_count Amount of items in array `physical_devices`.
 * @param out_count location into which the amount of suitable physical devices will be returned
 * @param out_physical_devices NULL or the array to output ordered list of devices into.
 * @return @returnstatus
 */
int thvk_EnumerateRankedPhysicalDevices(
    const thvk_RenderSystem_t *render_system,
    const VkPhysicalDevice *physical_devices,
    const unsigned int physical_device_count,
    unsigned int *const out_count,
    const VkPhysicalDevice **const out_physical_devices
);

/**
 * @ingroup vk_physical_device
 * @brief Get the indices of all queue families within the given Vulkan physical device.
 *
 * This function gets the indices of all queue families within the given physical device, and returns them in
 * a @ref thvk_QueueFamilyInfo_t structure.
 *
 * @param physical_device Physical device to query.
 * @return Struct containing queue family indices.
 */
thvk_QueueFamilyInfo_t thvk_GetQueueFamilyInfo(
    const VkPhysicalDevice physical_device
);

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif // !__thallium_vk_physical_device_h__
