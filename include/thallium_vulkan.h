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
#ifndef _THALLIUM_PUB_VULKAN_H
#define _THALLIUM_PUB_VULKAN_H
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

#include "thallium/vulkan/vk_extension.h"
#include "thallium/vulkan/vk_render_system.h"

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif // !_THALLIUM_PUB_VULKAN_H
