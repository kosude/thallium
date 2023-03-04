/*
 *   Copyright (c) 2023 Jack Bennett
 *   All rights reserved.
 *
 *   Please see the LICENCE file for more information.
 */

/**
 * @file vk_device.h
 * @brief Creating logical devices to interface with the most optimal physical device(s)
 *
 * @defgroup vk_device Creating Vulkan device handles
 * @brief Functions for creating Vulkan devices to interface with the most optimal physical device(s)
 * @ingroup vulkan
 *
 * This group contains functions involving Vulkan devices.
 */

#pragma once
#ifndef __thallium_vk_device_h__
#define __thallium_vk_device_h__
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

#include "thallium/fwdvk.h"

#include <volk/volk.h>


// ===========================================================================
//                           THALLIUM PUBLIC API
// ===========================================================================

/**
 * @ingroup vk_device
 * @brief Given an array of Vulkan physical devices, rank them and return the final sorted array (into `out_physical_devices`).
 *
 * This function sorts the given array of Vulkan physical devices in order of suitability, and returns the
 * sorted array. **The item at index `0` is the most suitable, and the item at index `\*physical_device_count - 1` is
 * the least suitable.**
 *
 * The length of the resultant array is the same as `physical_device_count`.
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
 * @param out_physical_devices NULL or the array to output ordered list of devices into.
 * @return @returnstatus
 */
int thvk_EnumerateRankedPhysicalDevices(
    const thvk_RenderSystem_t *const render_system,
    const VkPhysicalDevice *const physical_devices,
    const unsigned int physical_device_count,
    const VkPhysicalDevice **out_physical_devices
);

/**
 * @ingroup vk_device
 * @brief Create a Vulkan logical device handle.
 *
 * This function creates a logical device handle structure into the render system `render_system`.
 *
 * The resultant device interfaces with `physical_device`.
 *
 * @note This function does not check the suitability of `physical_device`. You should do this with
 * @ref thvk_EnumerateRankedPhysicalDevices() first.
 *
 * @param render_system Pointer to the render system into which the device will be returned.
 * @param physical_device Vulkan physical device structure for the logical device to represent
 * @return @returnstatus
 */
int thvk_CreateLogicalDevice(
    thvk_RenderSystem_t *render_system,
    const VkPhysicalDevice physical_device
);

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif // !__thallium_vk_device_h__
