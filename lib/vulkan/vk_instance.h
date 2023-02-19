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
#ifndef _THALLIUM_VULKAN_INSTANCE_H
#define _THALLIUM_VULKAN_INSTANCE_H
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

#include "thallium/vulkan/vk_render_system.h"

/// @cond DOCS_IGNORE

typedef struct th_Debugger_t th_Debugger_t;

/// @endcond DOCS_IGNORE

/**
 * @ingroup vk_instance
 * @brief Create a Vulkan instance.
 *
 * This function creates a Vulkan instance into the location `instance`.
 *
 * If `debugger` is not NULL, a debug messenger will be created alongside the instance for Vulkan debugging.
 *
 * @param out_instance Pointer to the location into which the Vulkan instance will be returned.
 * @param config Pointer to a Vulkan renderer configuration structure.
 * @param apiVersion Vulkan API version to create instance for (as a th_Version_t structure)
 * @param debugger NULL or a pointer to a Thallium debugger
 * @return @returnstatus
 */
const int thvk_CreateInstance(
    VkInstance *out_instance,
    const th_RendererConfigVulkan_t *config,
    const th_Version_t apiVersion,
    const th_Debugger_t *debugger
);

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif // !_THALLIUM_VULKAN_INSTANCE_H
