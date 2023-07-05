/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#pragma once
#ifndef __TL__internal__vulkan__vk_device_queues_h__
#define __TL__internal__vulkan__vk_device_queues_h__
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

#include <cutils/carray/carray.h>

// internal struct to hold queue handles returned from a logical device.
typedef struct TLVK_LogicalDeviceQueues_t {
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
