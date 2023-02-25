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
#include <string.h>




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
        r->config.application_name = malloc(strlen(config->application_name) * sizeof(char));
        thassert(r->config.application_name);
        strcpy(r->config.application_name, config->application_name);
        r->config.application_version = config->application_version;

        r->config.engine_name = malloc(strlen(config->engine_name) * sizeof(char));
        thassert(r->config.engine_name);
        strcpy(r->config.engine_name, config->engine_name);
        r->config.engine_version = config->engine_version;

        // first we copy the layer names onto the heap
        r->config.layer_names = malloc(config->layer_count * sizeof(const char *));
        thassert(r->config.layer_names);
        for (unsigned int i = 0; i < (unsigned int) config->layer_count; i++) {
            r->config.layer_names[i] = malloc(strlen(config->layer_names[i]) * sizeof(char));
            thassert(r->config.layer_names[i]);
            strcpy(r->config.layer_names[i], config->layer_names[i]);
        }
        r->config.layer_count = config->layer_count;

        // then repeat for the extension names
        r->config.extension_names = malloc(config->extension_count * sizeof(const char *));
        thassert(r->config.extension_names);
        for (unsigned int i = 0; i < (unsigned int) config->extension_count; i++) {
            r->config.extension_names[i] = malloc(strlen(config->extension_names[i]) * sizeof(char));
            thassert(r->config.extension_names[i]);
            strcpy(r->config.extension_names[i], config->extension_names[i]);
        }
        r->config.extension_count = config->extension_count;
    }

    // create Vulkan instance for render system
    thassert(thvk_CreateInstance(r));

    // TODO - just testing:

    unsigned int p_device_count, ranked_p_device_count;

    thassert_vk(vkEnumeratePhysicalDevices(r->instance, &p_device_count, NULL));
    VkPhysicalDevice p_devices[p_device_count];
    thassert_vk(vkEnumeratePhysicalDevices(r->instance, &p_device_count, p_devices));

    thassert(thvk_EnumerateRankedPhysicalDevices(r, p_devices, p_device_count, &ranked_p_device_count, NULL));
    const VkPhysicalDevice *ranked_p_devices[ranked_p_device_count];
    thassert(thvk_EnumerateRankedPhysicalDevices(r, p_devices, p_device_count, &ranked_p_device_count, ranked_p_devices));

    return r;
}

int thvk_DestroyRenderSystem(thvk_RenderSystem_t *render_system) {
    // this debug messenger was only created if debugger was present.
    if (render_system->debugger) {
        vkDestroyDebugUtilsMessengerEXT(render_system->instance, render_system->debug_messenger, NULL);
    }

    vkDestroyInstance(render_system->instance, NULL);

    free(render_system->config.application_name);
    free(render_system->config.engine_name);

    for (unsigned int i = 0; i < (unsigned int) render_system->config.layer_count; i++) {
        free(render_system->config.layer_names[i]);
    }
    free(render_system->config.layer_names);

    for (unsigned int i = 0; i < (unsigned int) render_system->config.extension_count; i++) {
        free(render_system->config.extension_names[i]);
    }
    free(render_system->config.extension_names);

    free(render_system);

    return 1;
}
