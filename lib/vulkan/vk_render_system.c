/*
 *   Copyright (c) 2023 Jack Bennett
 *   All rights reserved.
 *
 *   Please see the LICENCE file for more information.
 */

#include "thallium.h"

#include "utils/log.h"
#include "vk_instance.h"

#include <stdlib.h>




// TODO - just testing:
#include "thallium_vulkan.h"




#define FMT_VK_API_VERSION(vers) \
    VK_MAKE_API_VERSION( \
        0, \
        vers.major, \
        vers.minor, \
        vers.patch \
    )


// ===========================================================================
//                       THALLIUM PUBLIC API DEFINITIONS
// ===========================================================================

thvk_RenderSystem_t *thvk_CreateRenderSystem(const th_RendererConfig_Vulkan_t *config, const th_Version_t apiVersion, const th_Debugger_t *debugger) {
    thvk_RenderSystem_t *r = malloc(sizeof(thvk_RenderSystem_t));
    if (!r) {
        th_Fatal(debugger, "MALLOC fault in th_CreateRenderer!");
        return NULL;
    }

    r->apiVersion = FMT_VK_API_VERSION(apiVersion);

    // debugger can be NULL
    r->debugger = debugger;

    // create Vulkan instance for render system
    if (!thvk_CreateInstance(r, config)) {
        return NULL;
    }

    // TODO - just testing:
    unsigned int pdeviceCount = 0, rankedPDeviceCount = 0;
    VkPhysicalDevice *pdevices = thvk_GetAvailablePhysicalDevices(r, &pdeviceCount);
    VkPhysicalDevice *rankedPDevices = thvk_RankPhysicalDevices(pdevices, pdeviceCount, r, &rankedPDeviceCount);

    free(pdevices);
    free(rankedPDevices);

    return r;
}

const int thvk_DestroyRenderSystem(thvk_RenderSystem_t *renderSystem) {
    vkDestroyInstance(renderSystem->instance, NULL);

    return 1;
}
