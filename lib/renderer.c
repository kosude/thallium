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
    char apiVer[256]; \
    \
    th_VersionToString(descriptor.apiVersion, apiVer); \
    \
    th_Error(debugger, "Failed to create render system (th_CreateRenderer)"); \
    th_Hint(debugger, "API = \"%s\" (%s)", descriptor.apiName, apiVer); \
}

// Typedef for readability: different IDs to represent different graphics APIs.
typedef enum ApiId_t {
    THALLIUM_API_ID_NULL = 0x00,
    THALLIUM_API_ID_VULKAN = 0x01
} ApiId_t;


// ===========================================================================
//                       THALLIUM PUBLIC API DEFINITIONS
// ===========================================================================

th_Renderer_t *th_CreateRenderer(const th_RendererDescriptor_t descriptor, th_Debugger_t *debugger) {
    th_Renderer_t *r = malloc(sizeof(th_Renderer_t));
    if (!r) {
        th_Fatal(debugger, "MALLOC fault in th_CreateRenderer!");
        return NULL;
    }

    r->debugger = debugger;

    // vulkan renderers
    if (!strcmp(descriptor.apiName, "vulkan")) {
#       if defined(THALLIUM_VULKAN_INCL)
            r->apiId = THALLIUM_API_ID_VULKAN;

            // create the render system
            r->renderSystem = thvk_CreateRenderSystem((th_RendererConfigVulkan_t *) descriptor.rendererConfig, descriptor.apiVersion, debugger);
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

const void *th_GetRendererRenderSystem(const th_Renderer_t *renderer) {
    return renderer->renderSystem;
}
