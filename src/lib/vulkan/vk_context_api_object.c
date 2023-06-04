/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#include "lib/vulkan/vk_context_api_object.h"

#include "thallium.h"

#include "types/core/context.h"
#include "lib/core/context_api.h"
#include "utils/utils.h"

#include "lib/vulkan/vk_instance.h"

#include <volk/volk.h>

#include <stdlib.h>

// Format a Thallium version struct for Vulkan
#define __MAKE_API_VERSION(vers)        \
    VK_MAKE_API_VERSION(                \
        0,                              \
        vers.major,                     \
        vers.minor,                     \
        vers.patch                      \
    )

// flag to only load vulkan loader once
static bool __VOLK_INITIALISED = false;

bool TLVK_CreateContextVulkanBlock(TL_Context_t *const context, const TL_Version_t api_version, const TL_RendererFeatures_t features,
    const TL_Debugger_t *const debugger)
{
    if (!context) {
        return false;
    }

    if (context->vulkan_offset == TL_CONTEXT_API_OBJECT_UNINITIALISED) {
        TL_Warn(debugger, "TLVK_CreateContextVulkanBlock: Vulkan data block uninitialised for context object %p", context);
        return false;
    }

    TLVK_ContextVulkanBlock_t *block = (TLVK_ContextVulkanBlock_t *) ((char *) context->data + context->vulkan_offset);

    // block has already been initialised, so we return early.
    // we don't want to overwrite data in this block as we need to make sure not to lose e.g. the Vulkan instance handle.
    if (block->initialised) {
        return true;
    }

    if (!__VOLK_INITIALISED) {
        volkInitialize(); // seemed to have spelt initialise wrong?
        __VOLK_INITIALISED = true;
    }

    // creating the Vulkan instance...

    block->instance_layers = carraynew(8);
    block->instance_extensions = carraynew(16);

    VkApplicationInfo app_info = {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pNext = NULL,
        .pApplicationName = "Vulkan Application",
        .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
        .pEngineName = "No Engine",
        .engineVersion = VK_MAKE_VERSION(0, 0, 0),
        .apiVersion = __MAKE_API_VERSION(api_version),
    };

    block->vk_instance = TLVK_CreateInstance(app_info, features, &block->instance_layers, &block->instance_extensions, debugger);
    if (!block->vk_instance) {
        TL_Error(debugger, "Failed to create Vulkan instance in context %p (Vulkan block located at %p)", context, block);
        return false;
    }

    TL_Log(debugger, "Created Vulkan instance at %p in Thallium context %p", block->vk_instance, context);

    TL_Log(debugger, "  %d layers", block->instance_layers.size);
    for (uint32_t i = 0; i < block->instance_layers.size; i++) {
        TL_Log(debugger, "    - layer #%d: %s", i, block->instance_layers.data[i]);
    }
    TL_Log(debugger, "  %d instance-level extensions", block->instance_extensions.size);
    for (uint32_t i = 0; i < block->instance_extensions.size; i++) {
        TL_Log(debugger, "    - extension #%d: %s", i, block->instance_extensions.data[i]);
    }

    block->initialised = true;

    return true;
}

void TLVK_DestroyContextVulkanBlock(TL_Context_t *const context) {
    TLVK_ContextVulkanBlock_t *block = (TLVK_ContextVulkanBlock_t *) ((char *) context->data + context->vulkan_offset);

    // free members of the layer names array

    for (uint32_t i = 0; i < block->instance_layers.size; i++) {
        free((void *) block->instance_layers.data[i]);
    }
    carrayfree(&block->instance_layers);

    // free members of the instance extension names array
    for (uint32_t i = 0; i < block->instance_extensions.size; i++) {
        free((void *) block->instance_extensions.data[i]);
    }
    carrayfree(&block->instance_extensions);

    vkDestroyInstance(block->vk_instance, NULL);
}
