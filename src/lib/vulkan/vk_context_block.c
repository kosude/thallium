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

#include "lib/core/context_block.h"
#include "utils/utils.h"

#include "vk_instance.h"
#include "vk_loader.h"

#include <volk/volk.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define __MAKE_API_VERSION(vers)        \
    VK_MAKE_API_VERSION(                \
        0,                              \
        vers.major,                     \
        vers.minor,                     \
        vers.patch                      \
    )

#define __IF_INSTANCE_EXTENSION_ENABLED(ext, fn)                            \
for (uint32_t i = 0; i < iexts.size; i++) {                                 \
    if (!strcmp((const char *) iexts.data[i], ext)) {                       \
        fn                                                                  \
        break;                                                              \
    }                                                                       \
}


static VKAPI_ATTR VkBool32 VKAPI_CALL __DebugMessengerCallback(VkDebugUtilsMessageSeverityFlagBitsEXT severity,
    VkDebugUtilsMessageTypeFlagsEXT type, const VkDebugUtilsMessengerCallbackDataEXT *callback_data, void *user_data);

static VkDebugUtilsMessageSeverityFlagsEXT __ThalliumDebugSeveritiesToVulkanFlags(TL_DebugSeverityFlags_t severities);


bool TLVK_ContextBlockCreate(TL_Context_t *const context, const TL_Version_t api_version, TL_RendererFeatures_t *const features,
    const TL_Debugger_t *const debugger)
{
    if (!context) {
        return false;
    }

    if (context->vulkan_offset == TL_CONTEXT_BLOCK_UNINITIALISED) {
        TL_Warn(debugger, "TLVK_ContextBlockCreate: Vulkan data block uninitialised for context object %p", context);
        return false;
    }

    const TL_Debugger_t *dbgattached = context->attached_debugger;

    TLVK_ContextBlock_t *block = (TLVK_ContextBlock_t *) ((char *) context->data + context->vulkan_offset);

    // block has already been initialised, so we return early.
    // we don't want to overwrite data in this block as we need to make sure not to lose e.g. the Vulkan instance handle.
    if (block->state.initialised) {
        return true;
    }

    // init vulkan function loader
    TLVK_LoaderInit(debugger);

    // creating the Vulkan instance...

    VkApplicationInfo app_info = {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pNext = NULL,
        .pApplicationName = "thalliumbuilt",
        .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
        .pEngineName = "No Engine",
        .engineVersion = VK_MAKE_VERSION(0, 0, 0),
        .apiVersion = __MAKE_API_VERSION(api_version),
    };

    // this will be used to create a debug messenger for validation layers in TLVK_InstanceCreate, as well as the standalone debug messenger
    // (if a debug descriptor was specified to be attached)
    VkDebugUtilsMessengerCreateInfoEXT debug_utils_create_info = { 0 };

    if (dbgattached) {
        debug_utils_create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        debug_utils_create_info.pNext = NULL;
        debug_utils_create_info.flags = 0;
        debug_utils_create_info.messageSeverity = __ThalliumDebugSeveritiesToVulkanFlags(dbgattached->severities);
        debug_utils_create_info.messageType =       // all vulkan message types enabled
            VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
            | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT
            | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
            | VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT;
        debug_utils_create_info.pfnUserCallback = __DebugMessengerCallback;
        debug_utils_create_info.pUserData = (void *) dbgattached;
    }

    carray_t ilayers = carraynew(8);
    carray_t iexts = carraynew(16);

    VkInstance instance = TLVK_InstanceCreate(app_info, debug_utils_create_info, features, &ilayers, &iexts, debugger);
    if (!instance) {
        TL_Error(debugger, "Failed to create Vulkan instance in context %p (Vulkan block located at %p)", context, block);
        return false;
    }
    block->vk_instance = instance;
    block->instance_layers = ilayers;
    block->instance_extensions = iexts;

    if (debugger) {
        TL_Log(debugger, "Created Vulkan instance at %p in Thallium context %p", instance, context);

        TL_Log(debugger, "  %d layers", ilayers.size);
        for (uint32_t i = 0; i < ilayers.size; i++) {
            TL_Log(debugger, "    - layer #%d: %s", i, ilayers.data[i]);
        }
        TL_Log(debugger, "  %d extensions", iexts.size);
        for (uint32_t i = 0; i < iexts.size; i++) {
            TL_Log(debugger, "    - extension #%d: %s", i, iexts.data[i]);
        }
    }

    // load vulkan functions
    TLVK_LoaderInstanceLoad(instance);

    // creating the Vulkan messenger object (if debugger is attached)...

    // will be NULL if no debugger is attached
    block->vk_debug_messenger = VK_NULL_HANDLE;

    // check if the debug utils extension was enabled
    __IF_INSTANCE_EXTENSION_ENABLED(VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
        if (vkCreateDebugUtilsMessengerEXT(instance, &debug_utils_create_info, NULL, &block->vk_debug_messenger)) {
            TL_Error(debugger, "Failed to create Vulkan debug messenger in context %p (Vulkan block located at %p)", context, block);
            return false;
        }

        TL_Log(debugger, "Created Vulkan debug messenger at %p in Thallium context %p", block->vk_debug_messenger, context);
        TL_Log(debugger, "  Following filter configuration from debugger at location %p (via attachment to context)", dbgattached);
    );

    block->state.initialised = true;

    return true;
}

void TLVK_ContextBlockDestroy(TL_Context_t *const context) {
    TLVK_ContextBlock_t *block = (TLVK_ContextBlock_t *) ((char *) context->data + context->vulkan_offset);

    carray_t ilayers = block->instance_layers;
    carray_t iexts = block->instance_extensions;

    VkInstance instance = block->vk_instance;
    VkDebugUtilsMessengerEXT dbgmsg = block->vk_debug_messenger;

    // destroy the debug messenger if the function is available
    __IF_INSTANCE_EXTENSION_ENABLED(VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
        vkDestroyDebugUtilsMessengerEXT(instance, dbgmsg, NULL);
    );

    // free members of the layer names array
    for (uint32_t i = 0; i < ilayers.size; i++) {
        free((void *) ilayers.data[i]);
    }
    carrayfree(&block->instance_layers);

    // free members of the instance extension names array
    for (uint32_t i = 0; i < iexts.size; i++) {
        free((void *) iexts.data[i]);
    }
    carrayfree(&block->instance_extensions);

    vkDestroyInstance(instance, NULL);
}


static VKAPI_ATTR VkBool32 VKAPI_CALL __DebugMessengerCallback(VkDebugUtilsMessageSeverityFlagBitsEXT severity,
    VkDebugUtilsMessageTypeFlagsEXT type, const VkDebugUtilsMessengerCallbackDataEXT *callback_data, void *user_data)
{
    const char *msg = callback_data->pMessage;

    // userData is guaranteed to be a pointer to the Thallium debugger struct
    const TL_Debugger_t *debugger = (const TL_Debugger_t *) user_data;

    // convert the type to a string
    char type_str[32];
    memset(type_str, 0, sizeof(type_str));
    switch (type) {
        case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:
        default:
            snprintf(type_str, 32, "[g] ");
            break;
        case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:
            snprintf(type_str, 32, "[v] ");
            break;
        case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:
            snprintf(type_str, 32, "[o] ");
            break;
        case VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT:
            snprintf(type_str, 32, "[a] ");
            break;
    }

    // redirect to appropriate logging function based on severity
    switch (severity) {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            TL_Warn_Vk(debugger, "%s%s", type_str, msg);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            TL_Error_Vk(debugger, "%s%s", type_str, msg);
            break;
        default:
            TL_Log_Vk(debugger, "%s%s", type_str, msg);
            break;
    }

    return VK_FALSE;
}

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
