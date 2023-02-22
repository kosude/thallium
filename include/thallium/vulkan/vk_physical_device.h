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
 * @param renderSystem Render system for querying
 * @param physicalDevices Array of physical devices to copy and sort
 * @param physicalDeviceCount Amount of items in array `physicalDevices`.
 * @param out_count NULL or location into which the amount of suitable physical devices will be returned
 * @return Heap-allocated array of Vulkan physical device structures.
 */
VkPhysicalDevice *thvk_RankPhysicalDevices(
    const thvk_RenderSystem_t *renderSystem,
    const VkPhysicalDevice *physicalDevices,
    const unsigned int physicalDeviceCount,
    unsigned int *out_count
);

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif // !__thallium_vk_physical_device_h__
