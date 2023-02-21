/*
 *   Copyright (c) 2023 Jack Bennett
 *   All rights reserved.
 *
 *   Please see the LICENCE file for more information.
 */

/**
 * @file vk_instance.h
 * @brief Some functions for creating and handling Vulkan instances.
 *
 * @defgroup vk_instance Vulkan instance functions
 * @brief Internally used functions for managing Vulkan instances.
 * @ingroup vulkan
 *
 * Some functions for creating and handling Vulkan instances. These are frequently used when managing
 * Vulkan render systems.
 */

#pragma once
#ifndef __thallium_vk_instance_h__
#define __thallium_vk_instance_h__
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

#include "thallium/vulkan/vk_render_system.h"
#include "thallium/fwd.h"

/**
 * @ingroup vk_instance
 * @brief Create a Vulkan instance.
 *
 * This function creates a Vulkan instance into the location `instance`.
 *
 * If `debugger` is not NULL, a debug messenger will be created alongside the instance for Vulkan debugging.
 *
 * @param renderSystem Pointer to the render system into which the instance will be returned.
 * @param config Pointer to a Vulkan renderer configuration structure.
 * @return @returnstatus
 */
const int thvk_CreateInstance(
    thvk_RenderSystem_t *renderSystem,
    const th_RendererConfig_Vulkan_t *config
);

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif // !__thallium_vk_instance_h__
