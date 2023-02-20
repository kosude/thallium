/*
 *   Copyright (c) 2023 Jack Bennett
 *   All rights reserved.
 *
 *   Please see the LICENCE file for more information.
 */

#include "utils/log.h"
#include "vk_instance.h"

#include <stdlib.h>


// ===========================================================================
//                       THALLIUM PUBLIC API DEFINITIONS
// ===========================================================================

thvk_RenderSystem_t *thvk_CreateRenderSystem(const th_RendererConfig_Vulkan_t *config, const th_Version_t apiVersion, const th_Debugger_t *debugger) {
    thvk_RenderSystem_t *r = malloc(sizeof(thvk_RenderSystem_t));
    if (!r) {
        th_Fatal(debugger, "MALLOC fault in th_CreateRenderer!");
        return NULL;
    }

    r->debugger = debugger;

    // create Vulkan instance for render system
    if (!thvk_CreateInstance(&(r->instance), config, apiVersion, debugger)) {
        return NULL;
    }

    th_Note(debugger, "Created Vulkan render system at %p", r);

    return r;
}

const int thvk_DestroyRenderSystem(thvk_RenderSystem_t *renderSystem) {
    vkDestroyInstance(renderSystem->instance, NULL);

    return 1;
}
