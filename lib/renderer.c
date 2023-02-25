/*
 *   Copyright (c) 2023 Jack Bennett
 *   All rights reserved.
 *
 *   Please see the LICENCE file for more information.
 */

#include "thallium/core/renderer.h"

#include "cmake_modules.h"

#include "types.h"
#include "utils/log.h"

#include <stdlib.h>
#include <string.h>

// Typedef for readability: different IDs to represent different graphics APIs.
// This is not in renderer.h as it is not necessary to be in the public interface!
typedef enum ApiId_t {
    THALLIUM_API_ID_NULL = 0x00,
    THALLIUM_API_ID_VULKAN = 0x01
} ApiId_t;


// ===========================================================================
//                       THALLIUM PUBLIC API DEFINITIONS
// ===========================================================================

th_Renderer_t *th_CreateRenderer(const th_RendererDescriptor_t descriptor, const th_Debugger_t *const debugger) {
    th_Renderer_t *r = malloc(sizeof(th_Renderer_t));
    if (!r) {
        th_Fatal(debugger, "MALLOC fault in th_CreateRenderer!");
        return NULL;
    }

    r->debugger = (th_Debugger_t *) debugger;

    // vulkan renderers
    if (!strcmp(descriptor.api_name, "vulkan")) {
#       if defined(THALLIUM_VULKAN_INCL)
            r->api_id = THALLIUM_API_ID_VULKAN;

            // create the render system
            r->render_system = thvk_CreateRenderSystem(descriptor.api_version, debugger, (th_RendererConfig_Vulkan_t *) descriptor.renderer_config);
            if (!r->render_system) {
                th_Error(debugger, "Failed to create render system (th_CreateRenderer)");
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
        th_Error(debugger, "Invalid graphics API \"%s\" specified in th_CreateRenderer", descriptor.api_name);
        return NULL;
    }

    th_Note(debugger, "Successfully created renderer for API \"%s\" at %p", descriptor.api_name, r);

    return r;
}

int th_DestroyRenderer(th_Renderer_t *renderer) {
#   if defined(THALLIUM_VULKAN_INCL)
        if (renderer->api_id == THALLIUM_API_ID_VULKAN) {
            thvk_DestroyRenderSystem(renderer->render_system);
        }
#   endif // THALLIUM_VULKAN_INCL

    free(renderer);

    return 1;
}

void *th_GetRendererRenderSystem(const th_Renderer_t *renderer) {
    return renderer->render_system;
}
