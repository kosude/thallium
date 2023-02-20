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

#include "thallium/core/version.h"
#include "thallium/fwd.h"

#include <vulkan/vulkan.h>


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
} thvk_RenderSystem_t;

/**
 * @ingroup vk_render_system
 * @brief Create and return a heap-allocated Vulkan render system.
 *
 * This function creates a new Vulkan render system and returns it. If there were any errors in
 * creation, NULL will be returned instead.
 *
 * @param config Pointer to a Vulkan renderer configuration structure.
 * @param debugger Debugger to read configuration options from when posting debug messages
 * @param apiVersion Vulkan API version (as a Thallium @ref th_Version_t structure)
 * @return The new render system
 *
 * @sa @ref thvk_RenderSystem_t
 * @sa @ref thvk_DestroyRenderSystem()
 */
thvk_RenderSystem_t *thvk_CreateRenderSystem(
    const th_RendererConfig_Vulkan_t *config,
    const th_Version_t apiVersion,
    const th_Debugger_t *debugger
);

/**
 * @ingroup vk_render_system
 * @brief Free the given Vulkan render system object.
 *
 * This function frees the specified render system object. You should set the pointer to
 * NULL after calling this function, as you would normally when calling free().
 *
 * @param renderSystem Pointer to the Vulkan render system to free.
 * @return @returnstatus
 *
 * @alwaysok
 *
 * @sa @ref thvk_CreateRenderSystem()
 */
const int thvk_DestroyRenderSystem(
    thvk_RenderSystem_t *renderSystem
);

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif // !__thallium_vk_render_system_h__
