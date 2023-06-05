/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#pragma once
#ifndef __TL__internal__vulkan__vk_context_api_object_h__
#define __TL__internal__vulkan__vk_context_api_object_h__
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

#include "thallium/fwd.h"
#include "thallium/platform.h"

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.h>
#include <cutils/carray/carray.h>

/**
 * @brief The block of data stored in a Thallium context object referenced by Vulkan renderers.
 *
 * This struct describes the block of data stored in a Thallium context object referenced by Vulkan renderers.
 */
typedef struct TLVK_ContextVulkanBlock_t {
    /// @brief Has the Vulkan block already been initialised? Here to avoid overwriting existing handle to stuff like the Vulkan instance.
    bool initialised;

    /// @brief Vulkan instance object:
    /// https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkInstance.html
    VkInstance vk_instance;
    /// @brief Vulkan debug messenger object:
    /// https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkDebugUtilsMessengerEXT.html
    /// This object is only initialised if a debugger was 'attached' to the context
    VkDebugUtilsMessengerEXT vk_debug_messenger;

    /// @brief Array of enabled instance layers
    carray_t instance_layers;
    /// @brief Array of enabled instance extensions
    carray_t instance_extensions;
} TLVK_ContextVulkanBlock_t;

/**
 * @brief Populate a block of memory for a Vulkan API object to be stored as part of a Thallium context.
 *
 * This function populates a block of memory for a Vulkan API object that can be stored as part of a Thallium context.
 *
 * @param context Context in which the Vulkan block of data will be stored.
 * @param api_version The minimum version of the Vulkan API to request
 * @param features Renderer features to request
 * @param debugger NULL or a debugger for function debugging
 *
 * @return False if error
 */
bool TLVK_CreateContextVulkanBlock(
    TL_Context_t *const context,
    const TL_Version_t api_version,
    const TL_RendererFeatures_t features,
    const TL_DebuggerAttachmentDescriptor_t *const attached_debug_descriptor,
    const TL_Debugger_t *const debugger
);

/**
 * @brief Destroy Vulkan API objects stored in the Vulkan data block of the given Thallium context.
 *
 * This function destroys the Vulkan API objects stored in the Vulkan data block of the given Thallium context, such as the `VkInstance` object.
 *
 * Whether or not this data block is initialised is implicitly checked.
 *
 * @param context Context to free data from.
 */
void TLVK_DestroyContextVulkanBlock(
    TL_Context_t *const context
);

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif
