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
 * Some functions for creating and handling Vulkan instances.
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
 * This function creates a Vulkan instance into the render system `render_system`.
 *
 * If `render_system` includes a debugger, a debug messenger will also be created for it.
 *
 * @param render_system Pointer to the render system into which the instance will be returned.
 * @return @returnstatus
 */
int thvk_CreateInstance(
    thvk_RenderSystem_t *render_system
);

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif // !__thallium_vk_instance_h__
