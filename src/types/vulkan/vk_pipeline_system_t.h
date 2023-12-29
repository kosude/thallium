/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#pragma once
#ifndef __TL__internal__vulkan__vk_pipeline_system_t_h__
#define __TL__internal__vulkan__vk_pipeline_system_t_h__
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

#include "thallium/vulkan/vk_pipeline_system.h"

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.h>

typedef struct TLVK_PipelineSystem_t {
    /// @brief The parent Vulkan renderer system.
    const TLVK_RendererSystem_t *renderer_system;

    /// @brief Handle to a Vulkan pipeline state object:
    /// https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkPipeline.html
    VkPipeline pso;
} TLVK_PipelineSystem_t;

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif
