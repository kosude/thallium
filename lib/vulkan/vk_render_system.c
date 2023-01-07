/*
 *   Copyright (c) 2023 Jack Bennett
 *   All rights reserved.
 *
 *   Please see the LICENCE file for more information.
 */

#include "thallium.h"
#include "thallium_vulkan.h"

#include "utils/log.h"

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

static const int CreateInstance(VkInstance *instance, const thvk_RenderSystemDescriptor_t descriptor);

const int thvk_InitRenderSystem(thvk_RenderSystem_t *renderSystem, const thvk_RenderSystemDescriptor_t descriptor) {
    if (!renderSystem) {
        th_Error("Render system pointer parameter was NULL in thvk_InitRenderSystem");
        return 0;
    }

    if (!CreateInstance(&renderSystem->instance, descriptor)) {
        return 0;
    }

    th_Log("Initialised Vulkan render system: %p", renderSystem);

    return 1;
}

static const int CreateInstance(VkInstance *instance, const thvk_RenderSystemDescriptor_t descriptor) {
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

    if (vkCreateInstance(&instanceDescr, NULL, instance)) {
        return 0;
    }

    return 1;
}
