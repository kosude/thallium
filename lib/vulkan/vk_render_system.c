/*
 *   Copyright (c) 2023 Jack Bennett
 *   All rights reserved.
 *
 *   Please see the LICENCE file for more information.
 */

#include "thallium.h"
#include "thallium_vulkan.h"

#include "utils/log.h"

#include <stdio.h>
#include <stdlib.h>
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

static VKAPI_ATTR const VkBool32 VKAPI_CALL _InstanceDebugMessengerCallback(VkDebugUtilsMessageSeverityFlagBitsEXT severity,
    VkDebugUtilsMessageTypeFlagsEXT type, const VkDebugUtilsMessengerCallbackDataEXT *callbackData, void *userData)
{
    char msg[1024];
    memset(msg, 0, 1024);

    // convert the severity to a string
    char severityStr[15];
    switch (severity) {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
            snprintf(severityStr, 15, "LOG  ");
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
            snprintf(severityStr, 15, "NOTIF");
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            snprintf(severityStr, 15, "WARN ");
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
        default:
            snprintf(severityStr, 15, "ERROR");
            break;
    }

    // convert the type to string
    char typeStr[15];
    switch (type) {
        case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:
        default:
            snprintf(typeStr, 15, "GENERAL    ");
            break;
        case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:
            snprintf(typeStr, 15, "VALIDATION ");
            break;
        case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:
            snprintf(typeStr, 15, "PERFORMANCE");
            break;
    }

    // print the message

    // We do not use Thallium logging functions to avoid associating Vulkan messages with Thallium message severities.
    // Note that the severity filter for this callback was defined by the user separate from the severity filter for Thallium messages.

    printf("vk: %s msg, type %s: %s\n", severityStr, typeStr, callbackData->pMessage);

    // userData will be set to whether or not THALLIUM_VK_INSTANCE_DEBUG_MESSENGER_DETAILED was enabled or not
    if (!*(int *) userData) {
        return VK_FALSE;
    }

    // if detailed debug messenger was enabled for this render system:

    // current queue info
    printf("\t%d queue items\n", callbackData->queueLabelCount);
    for (unsigned int i = 0; i < callbackData->queueLabelCount; i++) {
        if (callbackData->pQueueLabels[i].pLabelName) {
            printf("%s\n", callbackData->pQueueLabels[i].pLabelName);
        }
    }

    // command buffer info
    printf("\t%d command buffer contents\n", callbackData->cmdBufLabelCount);
    for (unsigned int i = 0; i < callbackData->cmdBufLabelCount; i++) {
        if (callbackData->pCmdBufLabels[i].pLabelName) {
            printf("%s\n", callbackData->pCmdBufLabels[i].pLabelName);
        }
    }

    // related objects
    printf("\t%d objects\n", callbackData->objectCount);
    for (unsigned int i = 0; i < callbackData->objectCount; i++) {
        if (callbackData->pObjects[i].pObjectName) {
            printf("%s\n", callbackData->pObjects[i].pObjectName);
        }
    }

    return VK_FALSE;
}

static const int _CreateInstance(VkInstance *instance, const thvk_RenderSystemDescriptor_t descriptor) {
    // convert descriptor to app info
    VkApplicationInfo appInfo = {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pNext = NULL,
        .pApplicationName = descriptor.applicationName,
        .applicationVersion = FMT_VK_VERSION(descriptor.applicationVersion),
        .pEngineName = descriptor.engineName,
        .engineVersion = FMT_VK_VERSION(descriptor.engineVersion),
        .apiVersion = FMT_VK_API_VERSION(descriptor.apiVersion)
    };

    // create instance
    VkInstanceCreateInfo instanceDescr = {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
        .pApplicationInfo = &appInfo,
        .enabledLayerCount = descriptor.layerCount,
        .ppEnabledLayerNames = (const char *const *) descriptor.layerNames,
        .enabledExtensionCount = descriptor.instanceExtensionCount,
        .ppEnabledExtensionNames = (const char *const *) descriptor.instanceExtensionNames
    };

    // check if debug utils flags were specified
    // if they were not, then create the instnace and return now.
    if (!descriptor.debugMessengerSeverities && !descriptor.debugMessengerTypes) {
        if (vkCreateInstance(&instanceDescr, NULL, instance)) {
            return 0;
        }

        return 1;
    }

    // otherwise, define a debug messenger and append it onto the instance

    // debug messenger create info
    VkDebugUtilsMessengerCreateInfoEXT messengerDescr = {
        .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
        .pNext = NULL,
        .flags = 0, // NOTE: "reserved for future use"
        .messageSeverity = descriptor.debugMessengerSeverities,
        .messageType = descriptor.debugMessengerTypes,
        .pfnUserCallback = _InstanceDebugMessengerCallback,
        .pUserData = (void *) &descriptor.detailedDebugMessenger
    };

    instanceDescr.pNext = &messengerDescr;

    // then we create the instance
    if (vkCreateInstance(&instanceDescr, NULL, instance)) {
        return 0;
    }

    return 1;
}


// ===========================================================================
//                       THALLIUM PUBLIC API DEFINITIONS
// ===========================================================================

thvk_RenderSystem_t *thvk_CreateRenderSystem(const thvk_RenderSystemDescriptor_t descriptor, const th_Debugger_t *debugger) {
    thvk_RenderSystem_t *r = malloc(sizeof(thvk_RenderSystem_t));
    if (!r) {
        th_Fatal(debugger, "MALLOC fault in th_CreateRenderer!");
        return NULL;
    }

    if (!_CreateInstance(&r->instance, descriptor)) {
        return NULL;
    }

    th_Note(debugger, "Created Vulkan render system at %p", r);

    return r;
}

const int thvk_DestroyRenderSystem(thvk_RenderSystem_t *renderSystem) {
    vkDestroyInstance(renderSystem->instance, NULL);

    return 1;
}
