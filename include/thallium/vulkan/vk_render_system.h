/*
 *   Copyright (c) 2023 Jack Bennett
 *   All rights reserved.
 *
 *   Please see the LICENCE file for more information.
 */

/**
 * @file vk_render_system.h
 * @brief API-dependent render systems for Vulkan
 *
 * @defgroup vk_render_system Vulkan render systems
 * @brief Handling low-level Vulkan-specific render system structures
 * @ingroup vulkan
 *
 * This group involves types and functions specific to rendering with Vulkan.
 */

#pragma once
#ifndef __thallium_vk_render_system_h__
#define __thallium_vk_render_system_h__
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

#include "thallium/fwd.h"
#include "thallium/fwdvk.h"
#include "thallium/core/version.h"

#include "thallium/vulkan/vk_config.h"

#include <volk/volk.h>


// ===========================================================================
//                           THALLIUM PUBLIC API
// ===========================================================================

/**
 * @ingroup vk_render_system
 * @brief A render system to hold Vulkan-specific rendering data.
 *
 * This structure holds renderer data for rendering with Vulkan.
 *
 * For example, each Vulkan render system holds one Vulkan
 * [instance](https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkInstance.html).
 *
 * @sa @ref thvk_CreateRenderSystem()
 */
typedef struct thvk_RenderSystem_t {
    /// @brief Pointer to Thallium debugger config struct
    const th_Debugger_t *debugger;

    /// @brief Vulkan instance
    VkInstance instance;
    /// @brief Full debug messenger (created immediately after instance creation, in thvk_CreateInstance(), if `debugger` is not NULL)
    VkDebugUtilsMessengerEXT debug_messenger;

    /// @brief Device handle
    VkDevice device;

    /// @brief Vulkan API version (formatted with VK_MAKE_API_VERSION())
    int api_version;
    /// @brief Vulkan renderer (thus render system) configuration options
    thvk_VulkanRendererConfig_t config;
    /// @brief 1 if a config struct was specified in thvk_CreateRenderSystem(); 0 if not.
    int config_specified;
} thvk_RenderSystem_t;

/**
 * @ingroup vk_render_system
 * @brief Create and return a heap-allocated Vulkan render system.
 *
 * This function creates a new Vulkan render system and returns it. If there were any errors in
 * creation, NULL will be returned instead.
 *
 * @param api_version Vulkan API version (as a Thallium @ref th_Version_t structure)
 * @param debugger NULL or debugger to read configuration options from when posting debug messages
 * @param config A Vulkan renderer configuration structure.
 * @return The new render system
 *
 * @sa @ref thvk_RenderSystem_t
 * @sa @ref thvk_DestroyRenderSystem()
 */
thvk_RenderSystem_t *thvk_CreateRenderSystem(
    const th_Version_t api_version,
    const th_Debugger_t *const debugger,
    const thvk_VulkanRendererConfig_t *const config
);

/**
 * @ingroup vk_render_system
 * @brief Free the given Vulkan render system object.
 *
 * This function frees the specified render system object. You should set the pointer to
 * NULL after calling this function, as you would normally when calling free().
 *
 * @param render_system Pointer to the Vulkan render system to free.
 * @return @returnstatus
 *
 * @alwaysok
 *
 * @sa @ref thvk_CreateRenderSystem()
 */
int thvk_DestroyRenderSystem(
    thvk_RenderSystem_t *render_system
);

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif // !__thallium_vk_render_system_h__
