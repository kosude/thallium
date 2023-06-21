/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#pragma once
#ifndef __TL__internal__vulkan__vk_render_system_h__
#define __TL__internal__vulkan__vk_render_system_h__
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

#include "thallium/vulkan/vk_render_system.h"

#include "thallium/core/renderer.h"
#include "lib/vulkan/vk_context_api_object.h"

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.h>

typedef struct TLVK_RenderSystem_t {
    /// @brief The parent API-agnostic renderer object.
    const TL_Renderer_t *renderer;
    /// @brief Vulkan context data block pointer.
    const TLVK_ContextVulkanBlock_t *vk_context;
    /// @brief Stack copy of renderer features in `renderer` (used for less derefs)
    TL_RendererFeatures_t features;

    /// @brief Device manager object (contains Vulkan logical and physical device interface(s))
    TLVK_DeviceManager_t *device_manager;
} TLVK_RenderSystem_t;

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif
