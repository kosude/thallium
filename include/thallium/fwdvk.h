/*
 *   Copyright (c) 2023 Jack Bennett
 *   All rights reserved.
 *
 *   Please see the LICENCE file for more information.
 */

/**
 * @file fwdvk.h
 * @brief Thallium library Vulkan structure forward declarations
 *
 * Include this file to recieve forward declarations of public Thallium types within the Vulkan library
 * module. This may be useful if you want to avoid including @ref thallium_vulkan.h for whatever reason.
 */

#pragma once
#ifndef __thallium_fwdvk_h__
#define __thallium_fwdvk_h__
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

/** @cond */

typedef struct thvk_VulkanRendererConfig_t thvk_VulkanRendererConfig_t;
typedef struct thvk_VulkanRendererApplicationConfig_t thvk_VulkanRendererApplicationConfig_t;
typedef struct thvk_VulkanRendererExtensionConfig_t thvk_VulkanRendererExtensionConfig_t;
typedef struct thvk_VulkanRendererDeviceConfig_t thvk_VulkanRendererDeviceConfig_t;

typedef struct thvk_RenderSystem_t thvk_RenderSystem_t;

/** @endcond */

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif // !__thallium_fwdvk_h__
