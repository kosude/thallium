/*
 *   Copyright (c) 2023 Jack Bennett
 *   All rights reserved.
 *
 *   Please see the LICENCE file for more information.
 */

#include "thallium.h"

#include "assert.h"
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

thvk_RenderSystem_t *thvk_CreateRenderSystem(const th_Version_t api_version, const th_Debugger_t *debugger,
    const th_RendererConfig_Vulkan_t *config)
{
    thvk_RenderSystem_t *r = malloc(sizeof(thvk_RenderSystem_t));
    if (!r) {
        th_Fatal(debugger, "MALLOC fault in th_CreateRenderer!");
        return NULL;
    }

    r->api_version = FMT_VK_API_VERSION(api_version);

    // debugger can be NULL
    r->debugger = debugger;

    // create Vulkan instance for render system
    if (!thvk_CreateInstance(r, config)) {
        return NULL;
    }

    // TODO - just testing:

    unsigned int p_device_count = 0, ranked_p_device_count = 0;

    TH_ASSERT_VK(vkEnumeratePhysicalDevices(r->instance, &p_device_count, NULL));
    VkPhysicalDevice p_devices[p_device_count];
    TH_ASSERT_VK(vkEnumeratePhysicalDevices(r->instance, &p_device_count, p_devices));

    thvk_EnumerateRankedPhysicalDevices(r, p_devices, p_device_count, &ranked_p_device_count, NULL);
    const VkPhysicalDevice *ranked_p_devices[ranked_p_device_count];
    thvk_EnumerateRankedPhysicalDevices(r, p_devices, p_device_count, &ranked_p_device_count, ranked_p_devices);

    return r;
}

int thvk_DestroyRenderSystem(thvk_RenderSystem_t *render_system) {
    vkDestroyInstance(render_system->instance, NULL);

    free(render_system);

    return 1;
}
