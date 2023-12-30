/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#pragma once
#ifndef __TL__internal__vulkan__vk_swapchain_system_t_h__
#define __TL__internal__vulkan__vk_swapchain_system_t_h__
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

#include "thallium/vulkan/vk_swapchain_system.h"

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.h>

typedef struct TLVK_SwapchainSystem_t {
    /// @brief The parent Vulkan renderer system.
    const TLVK_RendererSystem_t *renderer_system;

    /// @brief VkInstance object, retrieved from the context of the parent renderer of renderer_system
    VkInstance vk_instance;

    /// @brief Vulkan swapchain object:
    /// https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkSwapchainKHR.html
    VkSwapchainKHR vk_swapchain;
    /// @brief Vulkan window surface object:
    /// https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkSurfaceKHR.html
    VkSurfaceKHR vk_surface;

    /// @brief Array of Vulkan images (colour buffers), retrieved from `vk_swapchain`:
    /// https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkImage.html
    VkImage *col_images;
    /// @brief Amount of elements in `vk_images`.
    uint32_t col_image_count;

    /// @brief Format of the swapchain's colour buffers/images
    VkFormat col_format;
    /// @brief Swapchain extent (resolution).
    VkExtent2D extent;
} TLVK_SwapchainSystem_t;

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif
