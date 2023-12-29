/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#pragma once
#ifndef __TL__internal__vulkan__vk_device_h__
#define __TL__internal__vulkan__vk_device_h__
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

#include "thallium_decl/fwd.h"
#include "thallium_decl/enumsvk.h"
#include "thallium/platform.h"

#include "lib/vulkan/vk_loader.h"
#include "types/vulkan/vk_device_queues_t.h"

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.h>

#include <cutils/carray/carray.h>

/**
 * @brief Create a Vulkan logical device object with the given extensions and capabilities enabled.
 *
 * This function creates a Vulkan logical device object with the given extensions and capabilities enabled.
 *
 * @note The given extensions/features/queue families are **not** validated in this extension.
 *
 * @param physical_device Physical device with which to interface
 * @param extensions Device-level extensions to request
 * @param features Device features to request
 * @param queue_families Queue families from which to request queues
 * @param out_queues NULL or a pointer to a struct into which the created queue handles will be returned
 * @param out_rfeatures A pointer to the renderer features struct - in case any features are found to be unavailable, this struct will be updated.
 * @param out_funcset A pointer to a function set, into which the function ptrs for this device will be loaded.
 * @param debugger Debugger object to debug the function with
 * @return VK_NULL_HANDLE if there was an error, otherwise return the resulting VkDevice.
 */
VkDevice TLVK_LogicalDeviceCreate(
    const VkPhysicalDevice physical_device,
    const carray_t extensions,
    const VkPhysicalDeviceFeatures features,
    const TLVK_PhysicalDeviceQueueFamilyIndices_t queue_families,
    TLVK_LogicalDeviceQueues_t *const out_queues,
    TL_RendererFeatures_t *const out_rfeatures,
    TLVK_FuncSet_t *const out_funcset,
    const TL_Debugger_t *const debugger
);

/**
 * @brief Return true if the given physical device can safely be used for the application.
 *
 * This function will return true if the given physical device can safely be used for the application based on the given requirements.
 *
 * @param physical_device Physical device to query from.
 * @param requirements Renderer requirements
 * @param debugger Debugger object to debug the function with
 * @return True if the device can be used, false if not
 */
bool TLVK_PhysicalDeviceCheckCandidacy(
    const VkPhysicalDevice physical_device,
    const TL_RendererFeatures_t requirements,
    const TL_Debugger_t *const debugger
);

/**
 * @brief Return the queue family indices from which queues must be requested from the given physical device based on requirements.
 *
 * This function returns the queue family indices from which queues must be requested from the given physical device based on requirements.
 *
 * @note -1 on any member of the returned struct indicates that no queues need be requested from that family.
 *
 * @param physical_device Physical device to query from.
 * @param requirements Renderer requirements
 * @return Queue families to request queues from
 */
TLVK_PhysicalDeviceQueueFamilyIndices_t TLVK_PhysicalDeviceQueueFamilyIndicesGetEnabled(
    const VkPhysicalDevice physical_device,
    const TL_RendererFeatures_t requirements
);

/**
 * @brief Select and return a physical device from an array of possible candidates.
 *
 * This function selects and returns the 'best' physical device from an array of possible candidates based on the given requirements.
 *
 * @param candidates Array of physical device choices.
 * @param requirements Renderer requirements
 * @param mode The select mode to run by
 * @param out_extensions Pointer to an array into which extensions are returned which can safely be enabled on the selected device.
 * @param out_features Pointer to a struct into which features are returned which can safely be enabled on the selected device.
 * @param debugger Debugger object to debug the function with
 * @return VK_NULL_HANDLE if there was an error, otherwise return the selected VkPhysicalDevice.
 */
VkPhysicalDevice TLVK_PhysicalDeviceSelect(
    const carray_t candidates,
    const TL_RendererFeatures_t requirements,
    const TLVK_PhysicalDeviceSelectionMode_t mode,
    carray_t *const out_extensions,
    VkPhysicalDeviceFeatures *const out_features,
    const TL_Debugger_t *const debugger
);

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif
