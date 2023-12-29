/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#pragma once
#ifndef __TL__internal__vulkan__vk_device_queues_t_h__
#define __TL__internal__vulkan__vk_device_queues_t_h__
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

#include "thallium/platform.h"

#include <cutils/carray/carray.h>

// internal struct to hold queue family indices as described by physical devices
typedef struct TLVK_PhysicalDeviceQueueFamilyIndices_t {
    /// @brief Graphics queue family index
    int32_t graphics;
    /// @brief Compute queue family index
    int32_t compute;
    /// @brief Memory transfer queue family index
    int32_t transfer;
    /// @brief Image presentation queue family index
    int32_t present;
} TLVK_PhysicalDeviceQueueFamilyIndices_t;

// internal struct to hold queue handles returned from a logical device.
typedef struct TLVK_LogicalDeviceQueues_t {
    /// @brief Index of the graphics queue family (-1 if not used)
    int32_t graphics_family;
    /// @brief Index of the compute queue family (-1 if not used)
    int32_t compute_family;
    /// @brief Index of the transfer queue family (-1 if not used)
    int32_t transfer_family;
    /// @brief Index of the present queue family (-1 if not used)
    int32_t present_family;

    /// @brief An array of queues for graphics operations
    carray_t graphics;
    /// @brief An array of queues for compute operations
    carray_t compute;
    /// @brief An array of queues for memory transfer operations
    carray_t transfer;
    /// @brief An array of queues for image presentation operations
    carray_t present;
} TLVK_LogicalDeviceQueues_t;

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif
