/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#pragma once
#ifndef __TL__vulkan__vk_device_manager_h__
#define __TL__vulkan__vk_device_manager_h__
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

#include "thallium/fwdvk.h"
#include "thallium/platform.h"

/**
 * @brief An object to store handles to a Vulkan physical and logical device.
 *
 * A Thallium Vulkan device manager stores handles to a Vulkan physical and logical device for use in Vulkan render systems.
 *
 * @sa @ref TLVK_CreateDeviceManager()
 * @sa @ref TLVK_DeviceData_t
 */
typedef struct TLVK_DeviceManager_t TLVK_DeviceManager_t;

/**
 * @brief Descriptor struct to configure the creation of a Thallium Vulkan [device manager](@ref TLVK_DeviceManager_t).
 *
 * This descriptor structure provides options for the creation of Thallium [device manager](@ref TLVK_DeviceManager_t) for Vulkan.
 */
typedef struct TLVK_DeviceManagerDescriptor_t {
    // TODO device manager descriptor
    // TODO - some way of overriding physical device selection and directly choosing the physical device (e.g. by name? index? idk ;-;)
    uint32_t placeholder;
} TLVK_DeviceManagerDescriptor_t;

/**
 * @brief Create and return a heap-allocated Vulkan device manager.
 *
 * This function creates a new Vulkan device manager, choosing and interfacing with GPU device(s) based on configuration given in `descriptor`. If
 * there were any errors during creation, NULL will be returned instead of the device manager object.
 *
 * @param render_system pointer to the parent render system object
 * @param descriptor Thallium Vulkan device manager descriptor
 * @return The new device manager
 *
 * @sa @ref TLVK_DeviceManager_t
 * @sa @ref TLVK_DestroyDeviceManager()
 */
TLVK_DeviceManager_t *TLVK_CreateDeviceManager(
    const TLVK_RenderSystem_t *const render_system,
    const TLVK_DeviceManagerDescriptor_t descriptor
);

/**
 * @brief Free the given Thallium Vulkan device management object.
 *
 * This function frees the specified device manager object.
 *
 * @param device_manager Pointer to the Thallium Vulkan device manager to free.
 *
 * @sa @ref TLVK_DeviceManager_t
 * @sa @ref TLVK_CreateDeviceManager()
 */
void TLVK_DestroyDeviceManager(
    TLVK_DeviceManager_t *const device_manager
);

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif
