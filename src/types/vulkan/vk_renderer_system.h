/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#pragma once
#ifndef __TL__internal__vulkan__vk_renderer_system_h__
#define __TL__internal__vulkan__vk_renderer_system_h__
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

#include "thallium/vulkan/vk_renderer_system.h"

#include "thallium/core/renderer.h"
#include "lib/vulkan/vk_loader.h"

#include "vk_device_queues.h"

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.h>

#include <cutils/carray/carray.h>

typedef struct TLVK_ContextBlock_t TLVK_ContextBlock_t; // forward decl for TLVK_RendererSystem_t

typedef struct TLVK_RendererSystem_t {
    /// @brief The parent API-agnostic renderer object.
    const TL_Renderer_t *renderer;
    /// @brief Vulkan context data block pointer.
    const TLVK_ContextBlock_t *vk_context;

    /// @brief Vulkan physical device object used by the renderer system.
    /// https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkPhysicalDevice.html
    VkPhysicalDevice vk_physical_device;
    /// @brief Handle to a Vulkan logical device.
    /// https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkDevice.html
    VkDevice vk_logical_device;

    /// @brief A function pointer table specific to device vk_logical_device.
    TLVK_FuncSet_t devfs;

    /// @brief Array of enabled device extensions
    carray_t device_extensions;
    /// @brief Enabled device features.
    /// https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkPhysicalDeviceFeatures.html
    VkPhysicalDeviceFeatures vk_device_features;

    /// @brief Vulkan queue handles:
    /// https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkQueue.html
    TLVK_LogicalDeviceQueues_t vk_queues;
} TLVK_RendererSystem_t;

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif
