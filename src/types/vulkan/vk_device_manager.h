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

typedef struct TLVK_DeviceManager_t {
    // TODO: vulkan device manager

    uint32_t placeholder;
} TLVK_DeviceManager_t;

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif
