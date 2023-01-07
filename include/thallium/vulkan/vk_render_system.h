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
 * @brief Handling low-level Vulkan-specific render systems
 * @ingroup vulkan
 *
 * This group involves types and functions specific to rendering with Vulkan.
 */

#pragma once
#ifndef _THALLIUM_PUB_VULKAN_RENDER_SYSTEM_H
#define _THALLIUM_PUB_VULKAN_RENDER_SYSTEM_H
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

#include "thallium/version.h"

#include <vulkan/vulkan.h>

/**
 * @ingroup vk_render_system
 * @brief A render system to hold Vulkan-specific rendering data.
 *
 * This structure holds renderer data for rendering with Vulkan.
 *
 * For example, each Vulkan render system holds one Vulkan
 * [instance](https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkInstance.html).
 *
 * @sa @ref thvk_InitRenderSystem()
 */
typedef struct thvk_RenderSystem_t {
    /// @brief Vulkan instance
    VkInstance instance;
} thvk_RenderSystem_t;

/**
 * @ingroup vk_render_system
 * @brief A structure describing a render system to be created.
 *
 * This structure describes a Vulkan render system to be created.
 */
typedef struct thvk_RenderSystemDescriptor_t {
    /// @brief The version of the API being represented
    th_Version_t apiVersion;
    /// @brief The name of the application
    const char *applicationName;
    /// @brief The application's version
    th_Version_t applicationVersion;
    /// @brief The name of the engine being used, if applicable
    const char *engineName;
    /// @brief The version of the engine being used, if applicable
    th_Version_t engineVersion;

    /// @brief Array of names of enabled instance extensions.
    const char **instanceExtensionNames;
    /// @brief The amount of elements in `instanceExtensionNames`.
    int instanceExtensionCount;
    /// @brief Array of names of enabled layers.
    const char **layerNames;
    /// @brief The amount of elements in `layerNames`.
    int layerCount;
} thvk_RenderSystemDescriptor_t;

/**
 * @ingroup vk_render_system
 * @brief Initialise the specified Vulkan render system.
 *
 * This function initialises the data held in the specified Vulkan render system.
 *
 * This function should NOT be invoked multiple times.
 *
 * @param renderSystem Pointer to the render system to initialise.
 * @param descriptor Description of the render system to initialise.
 * @return @returnstatus
 *
 * @sa @ref thvk_RenderSystem_t
 */
const int thvk_InitRenderSystem(
    thvk_RenderSystem_t *renderSystem,
    const thvk_RenderSystemDescriptor_t descriptor
);

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif // !_THALLIUM_PUB_VULKAN_RENDER_SYSTEM_H
