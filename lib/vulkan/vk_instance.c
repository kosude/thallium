/*
 *   Copyright (c) 2023 Jack Bennett
 *   All rights reserved.
 *
 *   Please see the LICENCE file for more information.
 */

#include "vk_instance.h"

#include "thallium/debug.h"
#include "thallium/renderer.h"
#include "types.h"

#include "utils/log.h"

#include <stdio.h>
#include <string.h>

#define FMT_VK_VERSION(vers) \
    VK_MAKE_VERSION( \
        vers.major, \
        vers.minor, \
        vers.patch \
    )

#define FMT_VK_API_VERSION(vers) \
    VK_MAKE_API_VERSION( \
        0, \
        vers.major, \
        vers.minor, \
        vers.patch \
    )

// Callback for instance debug messengers - redirects messages to Thallium debugger
// userData is a pointer to the Thallium debugger struct
static VKAPI_ATTR const VkBool32 VKAPI_CALL _InstanceDebugMessengerCallback(VkDebugUtilsMessageSeverityFlagBitsEXT severity,
    VkDebugUtilsMessageTypeFlagsEXT type, const VkDebugUtilsMessengerCallbackDataEXT *callbackData, void *userData)
{
    // convert the type to a string
    char typeStr[32];
    switch (type) {
        case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:
        default:
            snprintf(typeStr, 32, "GENERAL");
            break;
        case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:
            snprintf(typeStr, 32, "VALIDATION");
            break;
        case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:
            snprintf(typeStr, 32, "PERFORMANCE");
            break;
        case VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT:
            snprintf(typeStr, 32, "DEVICE_ADDRESS_BINDING");
            break;
    }

    // redirect to appropriate logging function based on severity
    switch (severity) {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            th_Warn(userData, "%s: %s", typeStr, callbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            th_Error(userData, "%s: %s", typeStr, callbackData->pMessage);
            break;
        default:
            th_Log(userData, "%s: %s", typeStr, callbackData->pMessage);
            break;
    }

    return VK_FALSE;
}

// Convert debug message severity flags from Thallium format to Vulkan format.
static const VkDebugUtilsMessageSeverityFlagsEXT _ThalliumDebugSeveritiesToVulkanFlags(th_DebugSeverity_t severities) {
    // NOTIF severity is not considerered as the only appropriate Vulkan severities (VERBOSE and INFO) are
    // output via th_Log(), which would not show if the VERBOSE severity is not enabled anyway.
    // i.e., even when considering NOTIF, nothing different is output, so there's no point.
    return
          (severities & THALLIUM_DEBUG_SEVERITY_VERBOSE_BIT) >> 4   // VERBOSE  --> VERBOSE
        | (severities & THALLIUM_DEBUG_SEVERITY_VERBOSE_BIT)        //              + INFO
        | (severities & THALLIUM_DEBUG_SEVERITY_WARNING_BIT) << 6   // WARNING  --> WARNING
        | (severities & THALLIUM_DEBUG_SEVERITY_ERROR_BIT) << 11    // ERROR    --> ERROR
        | (severities & THALLIUM_DEBUG_SEVERITY_FATAL_BIT) << 12;   // FATAL    --> ERROR
}

// Create a debug messenger (return its descriptor)
static const VkDebugUtilsMessengerCreateInfoEXT _CreateDebugMessenger(const th_Debugger_t *debugger) {
    return (VkDebugUtilsMessengerCreateInfoEXT) {
        .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
        .pNext = NULL,
        .flags = 0, // NOTE: "reserved for future use"
        .messageSeverity = _ThalliumDebugSeveritiesToVulkanFlags(debugger->debugSeverityFilter),
        .messageType =
            VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
            | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT
            | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
            | VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT,
        .pfnUserCallback = _InstanceDebugMessengerCallback,
        .pUserData = (void *) debugger
    };
}


// ===========================================================================
//                       THALLIUM PUBLIC API DEFINITIONS
// ===========================================================================

const int thvk_CreateInstance(VkInstance *instance, const th_RendererConfigVulkan_t *config, const th_Version_t apiVersion,
    const th_Debugger_t *debugger)
{
    VkApplicationInfo appInfo;
    VkInstanceCreateInfo instanceInfo;
    VkDebugUtilsMessengerCreateInfoEXT debugMessengerInfo;

    instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceInfo.flags = 0;

    // populate application info based on config if it was given
    if (config) {
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pNext = NULL;
        appInfo.pApplicationName = config->applicationName;
        appInfo.applicationVersion = FMT_VK_VERSION(config->applicationVersion);
        appInfo.pEngineName = config->engineName;
        appInfo.engineVersion = FMT_VK_VERSION(config->engineVersion);
        appInfo.apiVersion = FMT_VK_API_VERSION(apiVersion);

        instanceInfo.pApplicationInfo = &appInfo;
    } else {
        instanceInfo.pApplicationInfo = NULL;
    }

    // if a debugger was passed, then we can use it to describe a debug messenger.
    // this will require the VK_EXT_debug_utils to be enabled; this is handled as part of the
    // required layers and instance extensions checking.
    if (debugger) {
        debugMessengerInfo = _CreateDebugMessenger(debugger);
        instanceInfo.pNext = &debugMessengerInfo;
    } else {
        instanceInfo.pNext = NULL;
    }

    // TODO: see Trello: change Vulkan extensions design (required instance extensions)
    // i.e.: getting all supported layers/extensions, and filtering the non-required ones out.
    // required layers include ones optionally specified by the user in `config`.
    // required extensions include OS-dependent surface extensions and, conditionally, debug utils.
    instanceInfo.enabledLayerCount = 0;
    instanceInfo.enabledExtensionCount = 0;
    instanceInfo.ppEnabledLayerNames = NULL;
    instanceInfo.ppEnabledExtensionNames = NULL;

    // then we create the instance
    if (vkCreateInstance(&instanceInfo, NULL, instance)) {
        return 0;
    }

    return 1;
}
