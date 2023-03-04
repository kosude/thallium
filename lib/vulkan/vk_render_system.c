/*
 *   Copyright (c) 2023 Jack Bennett
 *   All rights reserved.
 *
 *   Please see the LICENCE file for more information.
 */

#include "thallium.h"
#include "thallium_vulkan.h"

#include "assert.h"
#include "utils/log.h"
#include "vk_instance.h"

#include <stdlib.h>
#include <string.h>

// convert thallium to vulkan version format
#define FMT_VK_API_VERSION(vers) \
    VK_MAKE_API_VERSION( \
        0, \
        vers.major, \
        vers.minor, \
        vers.patch \
    )

// copy SRC config struct into DEST
static void _CopyConfig(thvk_VulkanRendererConfig_t *dest, const thvk_VulkanRendererConfig_t src) {
    // copy application version and name str

    dest->application_config.application_name = malloc(strlen(src.application_config.application_name) * sizeof(char) + 1); // + 1 for null term
    thassert(dest->application_config.application_name);
    strcpy(dest->application_config.application_name, src.application_config.application_name);
    dest->application_config.application_version = src.application_config.application_version;

    // copy engine version and name str

    dest->application_config.engine_name = malloc(strlen(src.application_config.engine_name) * sizeof(char) + 1);
    thassert(dest->application_config.engine_name);
    strcpy(dest->application_config.engine_name, src.application_config.engine_name);
    dest->application_config.engine_version = src.application_config.engine_version;

    // copy layer name str array and count

    dest->extension_config.layer_names = malloc(src.extension_config.layer_count * sizeof(const char *));
    thassert(dest->extension_config.layer_names);
    for (unsigned int i = 0; i < (unsigned int) src.extension_config.layer_count; i++) {
        dest->extension_config.layer_names[i] = malloc(strlen(src.extension_config.layer_names[i]) * sizeof(char) + 1);
        thassert(dest->extension_config.layer_names[i]);
        strcpy(dest->extension_config.layer_names[i], src.extension_config.layer_names[i]);
    }
    dest->extension_config.layer_count = src.extension_config.layer_count;

    // copy extension name str array and count

    dest->extension_config.extension_names = malloc(src.extension_config.extension_count * sizeof(const char *));
    thassert(dest->extension_config.extension_names);
    for (unsigned int i = 0; i < (unsigned int) src.extension_config.extension_count; i++) {
        dest->extension_config.extension_names[i] = malloc(strlen(src.extension_config.extension_names[i]) * sizeof(char) + 1);
        thassert(dest->extension_config.extension_names[i]);
        strcpy(dest->extension_config.extension_names[i], src.extension_config.extension_names[i]);
    }
    dest->extension_config.extension_count = src.extension_config.extension_count;

    // copy device config

    dest->device_config = src.device_config;
}

// free the given config struct
static void _DestroyConfig(thvk_VulkanRendererConfig_t config) {
    free(config.application_config.application_name);
    free(config.application_config.engine_name);

    for (unsigned int i = 0; i < (unsigned int) config.extension_config.layer_count; i++) {
        free(config.extension_config.layer_names[i]);
    }
    free(config.extension_config.layer_names);

    for (unsigned int i = 0; i < (unsigned int) config.extension_config.extension_count; i++) {
        free(config.extension_config.extension_names[i]);
    }
    free(config.extension_config.extension_names);
}


// ===========================================================================
//                       THALLIUM PUBLIC API DEFINITIONS
// ===========================================================================

thvk_RenderSystem_t *thvk_CreateRenderSystem(const th_Version_t api_version, const th_Debugger_t *debugger,
    const thvk_VulkanRendererConfig_t *config)
{
    // load Vulkan pre-instance functions
    // if this assertion fails then Vulkan is not available/installed on the system (or at least the Vulkan libraries could not be found)

    // NOTE: volk's README says to avoid linking to vulkan-1 elsewhere (i.e. CMake) and this has not been considered - but no problems have been
    //       encountered as of writing.
    thassert_vk(volkInitialize());

    thvk_RenderSystem_t *r = malloc(sizeof(thvk_RenderSystem_t));
    if (!r) {
        th_Fatal(debugger, "MALLOC fault in th_CreateRenderer!");
        return NULL;
    }

    r->debugger = debugger; // (the debugger can be NULL)
    r->api_version = FMT_VK_API_VERSION(api_version);
    r->config_specified = config != NULL;

    if (config) {
        // copy the given config onto the heap within the render system's own config struct
        _CopyConfig(&(r->config), *config);
    }

    // create Vulkan instance for render system
    thassert(thvk_CreateInstance(r));

    // get physical devices and rank them
    unsigned int physical_device_count;
    vkEnumeratePhysicalDevices(r->instance, &physical_device_count, NULL);
    VkPhysicalDevice physical_devices[physical_device_count];
    vkEnumeratePhysicalDevices(r->instance, &physical_device_count, physical_devices);

    const VkPhysicalDevice *ranked_physical_devices[physical_device_count];

    if (!thvk_EnumerateRankedPhysicalDevices(r, physical_devices, physical_device_count, ranked_physical_devices)) {
        th_Fatal(debugger, "Couldn't create Vulkan render system without any suitable devices.");
    }

    // create most optimal logical device
    thassert(thvk_CreateLogicalDevice(r, *(ranked_physical_devices[0])));

    return r;
}

int thvk_DestroyRenderSystem(thvk_RenderSystem_t *render_system) {
    // this debug messenger was only created if debugger was present.
    if (render_system->debugger) {
        vkDestroyDebugUtilsMessengerEXT(render_system->instance, render_system->debug_messenger, NULL);
    }

    vkDestroyDevice(render_system->device, NULL);
    vkDestroyInstance(render_system->instance, NULL);

    _DestroyConfig(render_system->config);

    free(render_system);

    return 1;
}
