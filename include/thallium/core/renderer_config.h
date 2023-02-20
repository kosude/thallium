/*
 *   Copyright (c) 2023 Jack Bennett
 *   All rights reserved.
 *
 *   Please see the LICENCE file for more information.
 */

/**
 * @file renderer_config.h
 * @brief Optional renderer configuration
 *
 * @defgroup renderer_config Optional renderer configuration
 * @ingroup renderer
 * @brief Further configuring renderers for certain graphics APIs
 *
 * Descriptor structures for extra (mostly API-specific) configuration of [renderers](@ref renderer).
 */

#pragma once
#ifndef __thallium_renderer_config_h__
#define __thallium_renderer_config_h__
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

#include "thallium/core/version.h"


// ===========================================================================
//                           THALLIUM PUBLIC API
// ===========================================================================

/**
 * @ingroup renderer_config
 * @brief Configuration structure specifically for Vulkan renderers
 *
 * This structure provides exclusive configuration options specifically for Vulkan renderers.
 *
 * To apply options specified in this struct, pass it as a parameter in the @ref th_RendererDescriptor_t
 * descriptor which is passed to @ref th_CreateRenderer().
 *
 * @sa @ref th_CreateRenderer()
 * @sa @ref th_RendererDescriptor_t
 */
typedef struct th_RendererConfig_Vulkan_t {
    /// @brief The name of the application
    const char *applicationName;
    /// @brief The application's version
    th_Version_t applicationVersion;
    /// @brief The name of the engine being used, if applicable
    const char *engineName;
    /// @brief The version of the engine being used, if applicable
    th_Version_t engineVersion;

    /// @brief An array of names of Vulkan layers to enable
    const char **layerNames;
    /// @brief The amount of elements in `layerNames`
    int layerCount;
} th_RendererConfig_Vulkan_t;

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif // !__thallium_renderer_config_h__
