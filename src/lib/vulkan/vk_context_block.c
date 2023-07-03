/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#include "lib/vulkan/vk_context_block.h"

#include "thallium.h"

#include "types/core/context.h"
#include "types/core/debugger.h"

#include "lib/core/context_api.h"
#include "utils/utils.h"

#include "lib/vulkan/vk_instance.h"

#include <volk/volk.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Format a Thallium version struct for Vulkan
#define __MAKE_API_VERSION(vers)        \
    VK_MAKE_API_VERSION(                \
        0,                              \
        vers.major,                     \
        vers.minor,                     \
        vers.patch                      \
    )

// Callback for debug messengers - redirects messages to Thallium debugger
static VKAPI_ATTR VkBool32 VKAPI_CALL __DebugMessengerCallback(VkDebugUtilsMessageSeverityFlagBitsEXT severity,
    VkDebugUtilsMessageTypeFlagsEXT type, const VkDebugUtilsMessengerCallbackDataEXT *callback_data, void *user_data)
{
    // userData is guaranteed to be a pointer to the Thallium debugger struct
    const TL_Debugger_t *debugger = (const TL_Debugger_t *) user_data;

    // convert the type to a string
    char type_str[32];
    switch (type) {
        case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:
        default:
            snprintf(type_str, 32, "GENERAL");
            break;
        case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:
            snprintf(type_str, 32, "VALIDATION");
            break;
        case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:
            snprintf(type_str, 32, "PERFORMANCE");
            break;
        case VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT:
            snprintf(type_str, 32, "DEVICE_ADDRESS_BINDING");
            break;
    }

    // redirect to appropriate logging function based on severity
    switch (severity) {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            TL_Warn_Vk(debugger, "%s: %s", type_str, callback_data->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            TL_Error_Vk(debugger, "%s: %s", type_str, callback_data->pMessage);
            break;
        default:
            TL_Log_Vk(debugger, "%s: %s", type_str, callback_data->pMessage);
            break;
    }

    return VK_FALSE;
}

// Convert debug message severity flags from Thallium format to Vulkan format.
static VkDebugUtilsMessageSeverityFlagsEXT __ThalliumDebugSeveritiesToVulkanFlags(TL_DebugSeverityFlags_t severities) {
    // NOTIF severity is not considerered as the only appropriate Vulkan severities (VERBOSE and INFO) are
    // output by Thallium, which would not show if the VERBOSE severity is not enabled anyway.
    // i.e., even when considering NOTIF, nothing different is output, so there's no point.
    return
          (severities & TL_DEBUG_SEVERITY_VERBOSE_BIT)  >> 4    // VERBOSE  --> VERBOSE
        | (severities & TL_DEBUG_SEVERITY_VERBOSE_BIT)          //              + INFO
        | (severities & TL_DEBUG_SEVERITY_WARNING_BIT)  << 6    // WARNING  --> WARNING
        | (severities & TL_DEBUG_SEVERITY_ERROR_BIT)    << 11   // ERROR    --> ERROR
        | (severities & TL_DEBUG_SEVERITY_FATAL_BIT)    << 12;  // FATAL    --> ERROR
}


#define __IF_INSTANCE_EXTENSION_ENABLED(ext, fn)                            \
for (uint32_t i = 0; i < block->instance_extensions.size; i++) {            \
    if (!strcmp((const char *) block->instance_extensions.data[i], ext)) {  \
        fn                                                                  \
        break;                                                              \
    }                                                                       \
}

// flag to only load vulkan loader once
static bool __VOLK_INITIALISED = false;

bool TLVK_ContextBlockCreate(TL_Context_t *const context, const TL_Version_t api_version, const TL_RendererFeatures_t features,
    const TL_Debugger_t *const debugger)
{
    if (!context) {
        return false;
    }

    if (context->vulkan_offset == TL_CONTEXT_API_OBJECT_UNINITIALISED) {
        TL_Warn(debugger, "TLVK_ContextBlockCreate: Vulkan data block uninitialised for context object %p", context);
        return false;
    }

    TLVK_ContextBlock_t *block = (TLVK_ContextBlock_t *) ((char *) context->data + context->vulkan_offset);

    // block has already been initialised, so we return early.
    // we don't want to overwrite data in this block as we need to make sure not to lose e.g. the Vulkan instance handle.
    if (block->state.initialised) {
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

    // this will be used to create a debug messenger for validation layers in TLVK_InstanceCreate, as well as the standalone debug messenger
    // (if a debug descriptor was specified to be attached)
    VkDebugUtilsMessengerCreateInfoEXT debug_utils_create_info = { 0 };

    if (context->attached_debugger) {
        debug_utils_create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        debug_utils_create_info.pNext = NULL;
        debug_utils_create_info.flags = 0;
        debug_utils_create_info.messageSeverity = __ThalliumDebugSeveritiesToVulkanFlags(context->attached_debugger->severities);
        debug_utils_create_info.messageType =       // all vulkan message types enabled
            VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
            | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT
            | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
            | VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT;
        debug_utils_create_info.pfnUserCallback = __DebugMessengerCallback;
        debug_utils_create_info.pUserData = (void *) context->attached_debugger;
    }

    block->vk_instance = TLVK_InstanceCreate(app_info, debug_utils_create_info, features, &block->instance_layers, &block->instance_extensions,
        debugger);
    if (!block->vk_instance) {
        TL_Error(debugger, "Failed to create Vulkan instance in context %p (Vulkan block located at %p)", context, block);
        return false;
    }

    if (debugger) {
        TL_Log(debugger, "Created Vulkan instance at %p in Thallium context %p", block->vk_instance, context);

        TL_Log(debugger, "  %d layers", block->instance_layers.size);
        for (uint32_t i = 0; i < block->instance_layers.size; i++) {
            TL_Log(debugger, "    - layer #%d: %s", i, block->instance_layers.data[i]);
        }
        TL_Log(debugger, "  %d extensions", block->instance_extensions.size);
        for (uint32_t i = 0; i < block->instance_extensions.size; i++) {
            TL_Log(debugger, "    - extension #%d: %s", i, block->instance_extensions.data[i]);
        }
    }

    // load vulkan functions
    volkLoadInstance(block->vk_instance);

    // creating the Vulkan messenger object (if debugger is attached)...

    // will be NULL if no debugger is attached
    block->vk_debug_messenger = VK_NULL_HANDLE;

    // check if the debug utils extension was enabled
    __IF_INSTANCE_EXTENSION_ENABLED(VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
        if (vkCreateDebugUtilsMessengerEXT(block->vk_instance, &debug_utils_create_info, NULL, &block->vk_debug_messenger)) {
            TL_Error(debugger, "Failed to create Vulkan debug messenger in context %p (Vulkan block located at %p)", context, block);
            return false;
        }

        TL_Log(debugger, "Created Vulkan debug messenger at %p in Thallium context %p", block->vk_debug_messenger, context);
        TL_Log(debugger, "  Following filter configuration from debugger at location %p (via attachment to context)", context->attached_debugger);
    );

    block->state.initialised = true;

    return true;
}

void TLVK_ContextBlockDestroy(TL_Context_t *const context) {
    TLVK_ContextBlock_t *block = (TLVK_ContextBlock_t *) ((char *) context->data + context->vulkan_offset);

    // destroy the debug messenger if the function is available
    __IF_INSTANCE_EXTENSION_ENABLED(VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
        vkDestroyDebugUtilsMessengerEXT(block->vk_instance, block->vk_debug_messenger, NULL);
    );

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
