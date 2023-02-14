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
#ifndef _THALLIUM_PUB_VULKAN_RENDER_SYSTEM_H
#define _THALLIUM_PUB_VULKAN_RENDER_SYSTEM_H
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

#include "thallium/version.h"

#include <vulkan/vulkan.h>

// @cond DOCS_IGNORE

typedef struct th_Debugger_t th_Debugger_t;

// @endcond DOCS_IGNORE

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

    /// @brief If applicable, the severities to include in the instance debug messenger
    VkDebugUtilsMessageSeverityFlagBitsEXT debugMessengerSeverities;
    /// @brief If applicable, the types to include in the instance debug messenger
    VkDebugUtilsMessageTypeFlagBitsEXT debugMessengerTypes;
    /// @brief If applicable, should the instance debug messenger print detailed information?
    int detailedDebugMessenger;
} thvk_RenderSystemDescriptor_t;

/**
 * @ingroup vk_render_system
 * @brief Create and return a heap-allocated Vulkan render system.
 *
 * This function creates a new Vulkan render system and returns it. If there were any errors in
 * creation, NULL will be returned instead.
 *
 * @param descriptor Description of the render system to create.
 * @param debugger Debugger to read configuration options from when posting debug messages
 * @return The new render system
 *
 * @sa @ref thvk_RenderSystem_t
 * @sa @ref thvk_DestroyRenderSystem()
 */
thvk_RenderSystem_t *thvk_CreateRenderSystem(
    const thvk_RenderSystemDescriptor_t descriptor,
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
#endif // !_THALLIUM_PUB_VULKAN_RENDER_SYSTEM_H
