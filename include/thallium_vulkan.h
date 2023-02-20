/*
 *   Copyright (c) 2023 Jack Bennett
 *   All rights reserved.
 *
 *   Please see the LICENCE file for more information.
 */

/**
 * @file thallium_vulkan.h
 * @brief Thallium Vulkan module header
 *
 * You may include this file if you wish to directly use more low-level Vulkan
 * abstractions within Thallium. This is not recommended as it reduces API-agnosticism and
 * therefore code portability.
 *
 * @note To use functions included in this API, make sure you include the Vulkan module
 * when compiling the Thallium library with the `-DTHALLIUM_BUILD_MODULE_VULKAN=ON` flag.
 *
 * Thallium functions are prefixed with `thvk_`.
 */

#pragma once
#ifndef __thallium_vulkan_h__
#define __thallium_vulkan_h__
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

#include <vulkan/vulkan.h>

#include "thallium/vulkan/vk_extension.h"
#include "thallium/vulkan/vk_render_system.h"

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif // !__thallium_vulkan_h__
