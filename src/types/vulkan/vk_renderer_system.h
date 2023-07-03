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

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.h>

typedef struct TLVK_ContextBlock_t TLVK_ContextBlock_t;

typedef struct TLVK_RendererSystem_t {
    /// @brief The parent API-agnostic renderer object.
    const TL_Renderer_t *renderer;
    /// @brief Vulkan context data block pointer.
    const TLVK_ContextBlock_t *vk_context;
    /// @brief Stack copy of renderer features in `renderer` (used for less derefs)
    TL_RendererFeatures_t features;

    /// @brief Device manager object (contains Vulkan logical and physical device interface(s))
    TLVK_DeviceManager_t *device_manager;
} TLVK_RendererSystem_t;

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif
