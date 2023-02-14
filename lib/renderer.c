/*
 *   Copyright (c) 2023 Jack Bennett
 *   All rights reserved.
 *
 *   Please see the LICENCE file for more information.
 */

#include "thallium.h"

#include "cmake_modules.h"

#include "types.h"
#include "utils/log.h"

#include <stdlib.h>
#include <string.h>

#define RENDER_SYSTEM_CREATE_ERROR(debugger) { \
    char apiVer[256], appVer[256], engVer[256]; \
    \
    th_VersionToString(descriptor.apiVersion, apiVer); \
    th_VersionToString(descriptor.applicationVersion, appVer); \
    th_VersionToString(descriptor.engineVersion, engVer); \
    \
    th_Error(debugger, "Failed to create render system (th_CreateRenderer)"); \
    th_Hint(debugger, "API = \"%s\" (%s)", descriptor.apiName, apiVer); \
    th_Hint(debugger, "Application name = %s", descriptor.applicationName); \
    th_Hint(debugger, "Application version = %s", appVer); \
    th_Hint(debugger, "Engine name = %s", descriptor.engineName); \
    th_Hint(debugger, "Engine version = %s", engVer); \
}

// Typedef for readability: different IDs to represent different graphics APIs.
typedef enum ApiId_t {
    THALLIUM_API_ID_NULL = 0x00,
    THALLIUM_API_ID_VULKAN = 0x01
} ApiId_t;

#if defined(THALLIUM_VULKAN_INCL)
    static const VkDebugUtilsMessageSeverityFlagBitsEXT _GetVkDebugMessengerSeverityFlags(const th_RendererDescriptor_t descriptor) {
        int flags = descriptor.extensionDescriptor.vulkan->flags;

        return
            (flags & THALLIUM_VK_INSTANCE_DEBUG_MESSENGER_SEV_LOG_BIT) << 0
            | (flags & THALLIUM_VK_INSTANCE_DEBUG_MESSENGER_SEV_INFO_BIT) << 3
            | (flags & THALLIUM_VK_INSTANCE_DEBUG_MESSENGER_SEV_WARNING_BIT) << 6
            | (flags & THALLIUM_VK_INSTANCE_DEBUG_MESSENGER_SEV_ERROR_BIT) << 9;
    }

    static const VkDebugUtilsMessageTypeFlagBitsEXT _GetVkDebugMessengerTypeFlags(const th_RendererDescriptor_t descriptor) {
        int flags = descriptor.extensionDescriptor.vulkan->flags;

        return
            (flags & THALLIUM_VK_INSTANCE_DEBUG_MESSENGER_TYPE_GENERAL_BIT) >> 4
            | (flags & THALLIUM_VK_INSTANCE_DEBUG_MESSENGER_TYPE_VALIDATION_BIT) >> 4
            | (flags & THALLIUM_VK_INSTANCE_DEBUG_MESSENGER_TYPE_PERFORMANCE_BIT) >> 4
            | (flags & THALLIUM_VK_INSTANCE_DEBUG_MESSENGER_TYPE_DEVICE_BIND_BIT) >> 4;
    }
#endif


// ===========================================================================
//                       THALLIUM PUBLIC API DEFINITIONS
// ===========================================================================

th_Renderer_t *th_CreateRenderer(const th_RendererDescriptor_t descriptor, th_Debugger_t *debugger) {
    th_Renderer_t *r = malloc(sizeof(th_Renderer_t));
    if (!r) {
        th_Fatal(debugger, "MALLOC fault in th_CreateRenderer!");
        return NULL;
    }

    // copy debugger pointer to renderer for future functions
    r->debugger = debugger;

    // vulkan renderers
    if (!strcmp(descriptor.apiName, "vulkan")) {
#       if defined(THALLIUM_VULKAN_INCL)
            r->apiId = THALLIUM_API_ID_VULKAN;

            th_VulkanRendererExtensionDescriptor_t extensionDescriptor = *descriptor.extensionDescriptor.vulkan;

            // vulkan render system
            thvk_RenderSystemDescriptor_t renderSystemDescr = {
                .apiVersion = descriptor.apiVersion,
                .applicationName = descriptor.applicationName,
                .applicationVersion = descriptor.applicationVersion,
                .engineName = descriptor.engineName,
                .engineVersion = descriptor.engineVersion,

                .instanceExtensionNames = extensionDescriptor.extensionNames,
                .instanceExtensionCount = extensionDescriptor.extensionCount,
                .layerNames = extensionDescriptor.layerNames,
                .layerCount = extensionDescriptor.layerCount,

                .debugMessengerSeverities = _GetVkDebugMessengerSeverityFlags(descriptor),
                .debugMessengerTypes = _GetVkDebugMessengerTypeFlags(descriptor),
                .detailedDebugMessenger = extensionDescriptor.flags & THALLIUM_VK_INSTANCE_DEBUG_MESSENGER_DETAILED
            };

            // create the render system
            r->renderSystem = thvk_CreateRenderSystem(renderSystemDescr, debugger);
            if (!r->renderSystem) {
                RENDER_SYSTEM_CREATE_ERROR(debugger);
                return NULL;
            }
#       else
            th_Error(debugger, "th_CreateRenderer attempted to create renderer for API which was not compiled (\"%s\")", descriptor.apiName);
            th_Hint(debugger, "Recompile Thallium with the -DTHALLIUM_BUILD_MODULE_VULKAN=ON flag!");
            return NULL;
#       endif
    }
    // other/invalid renderers
    else {
        th_Error(debugger, "Invalid graphics API \"%s\" specified in th_CreateRenderer", descriptor.apiName);
        return NULL;
    }

    return r;
}

const int th_DestroyRenderer(th_Renderer_t *renderer) {
#   if defined(THALLIUM_VULKAN_INCL)
        if (renderer->apiId == THALLIUM_API_ID_VULKAN) {
            thvk_DestroyRenderSystem(renderer->renderSystem);
        }
#   endif // THALLIUM_VULKAN_INCL

    free(renderer->renderSystem);
    free(renderer);

    return 1;
}
