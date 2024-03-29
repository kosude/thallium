/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#include "thallium/core/renderer.h"
#include "types/core/renderer_t.h"

#include "lib/core/context_block.h"
#include "types/core/context_t.h"
#include "utils/utils.h"

#include "api_modules.h"

#include <stdlib.h>
#include <string.h>

static bool __ValidateAPI(const TL_RendererAPIFlags_t api, const TL_Debugger_t *const debugger);

static const char *__StringifyAPI(const TL_RendererAPIFlags_t api);

static void __AddToCombinedFeatures(TL_RendererFeatures_t *const base, const TL_RendererFeatures_t features);

static void __AddToCombinedAPIs(TL_RendererAPIFlags_t *const base, const TL_RendererAPIFlags_t api);

static void __ConsiderAPIVersion(TL_ContextAPIVersions_t *const base, const TL_RendererAPIFlags_t api, const TL_Version_t version);

static TL_Renderer_t *__CreateRenderer(TL_Context_t *const context, const TL_RendererDescriptor_t descriptor, const TL_Debugger_t *const debugger);


uint32_t TL_RendererCreate(TL_Context_t *const context, const uint32_t count, const TL_RendererDescriptor_t *const descriptors,
    TL_Renderer_t **const *const renderers, const TL_Debugger_t *const debugger)
{
    if (!context || !descriptors || !renderers || count <= 0) {
        return 0;
    }

    bool renderers_init = context->state.renderers_init;

    // function has already been called once on this context
    if (renderers_init) {
        TL_Warn(debugger, "Attempted to invoke TL_RendererCreate multiple times on the same context, which is illegal behaviour");
        return count;
    }

    // validate renderer apis
    for (uint32_t i = 0; i < count; i++) {
        TL_RendererAPIFlags_t api = descriptors[i].api;

        if (!__ValidateAPI(api, debugger)) {
            TL_Error(debugger, "Renderer descriptor at index %d in call to TL_RendererCreate specified invalid graphics API id %d", i, api);
            return 0;
        }
    }

    TL_RendererAPIFlags_t combined_apis = TL_RENDERER_API_NULL_BIT;
    TL_RendererFeatures_t combined_features = { 0 };
    // hopefully the abstracted APIs are backwards compatible as we only request the highest version specified :)
    TL_ContextAPIVersions_t highest_api_versions = { 0 };

    // combine all renderer information into single variables declared above
    for (uint32_t i = 0; i < count; i++) {
        TL_RendererDescriptor_t descriptor = descriptors[i];

        // combine info
        __AddToCombinedAPIs(&combined_apis, descriptor.api);
        __AddToCombinedFeatures(&combined_features, descriptor.requirements);
        __ConsiderAPIVersion(&highest_api_versions, descriptor.api, descriptor.api_version);

        TL_Log(debugger, "Context %p: renderer #%d - API id %d, version %d.%d.%d", context, i, descriptor.api, descriptor.api_version.major,
            descriptor.api_version.minor, descriptor.api_version.patch);
    }

    if (!TL_ContextBlocksCreate(context, combined_apis, highest_api_versions, &combined_features, debugger)) {
        TL_Error(debugger, "Failed to create API objects for context at %p in call to TL_RendererCreate", context);
        return 0;
    }

    TL_Log(debugger, "Allocated and populated context data for %d renderers", count);

    // create each renderer object
    uint32_t fail_renderer_count = 0;

    for (uint32_t i = 0; i < count; i++) {
        TL_RendererDescriptor_t descr = descriptors[i];

        // update descriptor presentation requirement in case it is not available
        if (descr.requirements.presentation) descr.requirements.presentation = combined_features.presentation;

        TL_Renderer_t *ret = __CreateRenderer(context, descr, debugger);
        if (!ret) {
            TL_Error(debugger, "TL_RendererCreate failed to create renderer index %d", i);
            fail_renderer_count++;
            continue;
        }

        // update pointer in given array
        *(renderers[i]) = ret;
    }

    uint32_t ret = count - fail_renderer_count;

    if (ret < count) {
        TL_Warn(debugger, "Call to TL_RendererCreate failed to create %d out of %d renderer(s) with context %p", fail_renderer_count, count,
            context);
    }

    if (ret > 0) {
        context->state.renderers_init = true;
        TL_Note(debugger, "Successfully created %d out of %d renderers with context %p", ret, count, context);
    }

    return ret;
}

void TL_RendererDestroy(TL_Renderer_t *const renderer) {
    if (!renderer) {
        return;
    }

    TL_RendererAPIFlags_t api = renderer->api;

    switch (api) {
        // destroy Vulkan renderer system
        case TL_RENDERER_API_VULKAN_BIT:;
#           if defined(_THALLIUM_VULKAN_INCL)
                void *renderersys = renderer->renderer_system;

                TLVK_RendererSystemDestroy((TLVK_RendererSystem_t *) renderersys);
#           endif
            break;

        case TL_RENDERER_API_NULL_BIT:
        default:
            break;
    }

    free(renderer);
}


static bool __ValidateAPI(const TL_RendererAPIFlags_t api, const TL_Debugger_t *const debugger) {
    switch (api) {
        case TL_RENDERER_API_VULKAN_BIT:
            // test if Vulkan module was compiled
#           if !defined(_THALLIUM_VULKAN_INCL)
                TL_Error(debugger, "TL_RendererCreate attempted to create renderer for API which was not compiled (TL_RENDERER_API_VULKAN_BIT); "
                    "Recompile Thallium with the -DTHALLIUM_BUILD_MODULE_VULKAN=ON flag!");
                return false;
#           else
                return true;
#           endif
        case TL_RENDERER_API_NULL_BIT:
        default:
            return false;
    }

    return false;
}

static const char *__StringifyAPI(const TL_RendererAPIFlags_t api) {
    switch (api) {
        case TL_RENDERER_API_VULKAN_BIT:
            return "Vulkan";
        case TL_RENDERER_API_NULL_BIT:
        default:
            return "Unknown";
    }

    return "Unknown";
}

static void __AddToCombinedFeatures(TL_RendererFeatures_t *const base, const TL_RendererFeatures_t features) {
    // using if statements means the base bools only get set to true, not to false
    // maybe there's a more optimal way of doing this

    if (features.presentation)
        base->presentation = true;
}

static void __AddToCombinedAPIs(TL_RendererAPIFlags_t *const base, const TL_RendererAPIFlags_t api) {
    *base |= api;
}

static void __ConsiderAPIVersion(TL_ContextAPIVersions_t *const base, const TL_RendererAPIFlags_t api, const TL_Version_t version) {
    // pointer to the version specified in `base` corresponding to the given api.
    TL_Version_t *base_version;

    switch (api) {
        case TL_RENDERER_API_VULKAN_BIT:
            base_version = &base->vulkan_version;
            break;
        case TL_RENDERER_API_NULL_BIT:
        default:
            return;
    }

    // check if any version values are higher than the base version
    bool is_greater =
        (version.major > base_version->major) |
        (version.minor > base_version->minor) |
        (version.patch > base_version->patch);

    // update base version if the specified one is greater
    if (is_greater) {
        *base_version = version;
    }
}

static TL_Renderer_t *__CreateRenderer(TL_Context_t *const context, const TL_RendererDescriptor_t descriptor, const TL_Debugger_t *const debugger) {
    if (!context) {
        return NULL;
    }

    TL_Renderer_t *renderer = malloc(sizeof(TL_Renderer_t));
    if (!renderer) {
        TL_Fatal(debugger, "MALLOC fault in call to __CreateRenderer");
        return NULL;
    }

    TL_Log(debugger, "Allocated renderer at %p", renderer);

    renderer->api = descriptor.api;
    renderer->context = context;
    renderer->debugger = context->attached_debugger;
    renderer->features = descriptor.requirements;

    // creating API-appropriate renderer system
    switch (descriptor.api) {

        // create a Vulkan renderer system...
        case TL_RENDERER_API_VULKAN_BIT:; // the semicolon somehow fixes variable declaration errors (fml)
#           if defined(_THALLIUM_VULKAN_INCL)

                TLVK_RendererSystemDescriptor_t rsdescr;

                if (descriptor.renderer_system_descriptor) {
                    rsdescr = *((TLVK_RendererSystemDescriptor_t *) descriptor.renderer_system_descriptor);
                } else {
                    // default renderer system descriptor configuration (used if no user-given descriptor was specified)...

                    rsdescr.physical_device_mode = TLVK_PHYSICAL_DEVICE_SELECTION_MODE_OPTIMAL;
                }

                TLVK_RendererSystem_t *renderersys = TLVK_RendererSystemCreate(renderer, rsdescr);
                if (!renderersys) {
                    TL_Error(debugger, "Failed to create Vulkan renderer system for new renderer at %p", renderer);
                    return NULL;
                }

                renderer->renderer_system = (void *) renderersys;

#           endif
            break;

        case TL_RENDERER_API_NULL_BIT:
        default:
            return NULL;

    }

    // printing debug information...

    TL_Note(debugger, "Renderer %p created, top-level information:", renderer);

    // enclosing if statement to avoid extra debug processing if its not necessary
    if (debugger) {
        TL_Note(debugger, "  API: %s (id %d)", __StringifyAPI(descriptor.api), descriptor.api);
    }

    TL_Note(debugger, "  Child renderer system: %p", renderer->renderer_system);

    return renderer;
}
