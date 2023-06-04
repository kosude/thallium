/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#include "thallium.h"

#include "types/core/context.h"
#include "types/core/renderer.h"

#include "utils/utils.h"
#include "lib/core/context_api.h"

#include <string.h>

static bool __ValidateAPI(const TL_RendererAPIFlags_t api, const TL_Debugger_t *const debugger) {
    switch (api) {
        case TL_RENDERER_API_VULKAN_BIT:
            // test if Vulkan module was compiled
#           if !defined(THALLIUM_VULKAN_INCL)
                TL_Error(debugger, "TL_CreateRenderers attempted to create renderer for API which was not compiled (TL_RENDERER_API_VULKAN_BIT)");
                TL_Hint(debugger, "Recompile Thallium with the -DTHALLIUM_BUILD_MODULE_VULKAN=ON flag!");
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

static void __AddToCombinedFeatures(TL_RendererFeatures_t *const base, const TL_RendererFeatures_t features) {
    base->placeholder = features.placeholder;
}

static void __AddToCombinedAPIs(TL_RendererAPIFlags_t *const base, const TL_RendererAPIFlags_t api) {
    (*base) |= api;
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
        version.major > base_version->major
        | version.minor > base_version->minor
        | version.patch > base_version->patch;

    // update base version if the specified one is greater
    if (is_greater) {
        *base_version = version;
    }
}

bool TL_CreateRenderers(TL_Context_t *const context, const uint32_t count, const TL_RendererDescriptor_t *const descriptors,
    TL_Renderer_t **renderers, const TL_Debugger_t *const debugger)
{
    if (!context || !descriptors || !renderers || count < 1) {
        return false;
    }

    // function has already been called once on this context
    if (context->renderers_init) {
        TL_Warn(debugger, "Attempted to invoke TL_CreateRenderers multiple times on the same context, which is illegal behaviour");
        return true;
    }

    // validate renderer apis
    for (uint32_t i = 0; i < count; i++) {
        TL_RendererAPIFlags_t api = descriptors[i].api;

        if (!__ValidateAPI(api, debugger)) {
            TL_Error(debugger, "Renderer descriptor at index %d in call to TL_CreateRenderers specified invalid graphics API id %d", i, api);
            return false;
        }
    }

    TL_RendererAPIFlags_t combined_apis = TL_RENDERER_API_NULL_BIT;
    TL_RendererFeatures_t combined_features = { 0 };

    // hopefully the abstracted APIs are backwards compatible as we only request the highest version specified :)
    TL_ContextAPIVersions_t highest_api_versions = { 0 };

    // combine all renderer information into single variables declared above
    for (uint32_t i = 0; i < count ; i++) {
        TL_RendererDescriptor_t descriptor = descriptors[i];

        __AddToCombinedFeatures(&combined_features, descriptor.requirements);
        __AddToCombinedAPIs(&combined_apis, descriptor.api);
        __ConsiderAPIVersion(&highest_api_versions, descriptor.api, descriptor.api_version);

        TL_Log(debugger, "Context %p: renderer #%d - API id %d, version %d.%d.%d", context, i, descriptor.api, descriptor.api_version.major,
            descriptor.api_version.minor, descriptor.api_version.patch);
    }

    if (!TL_CreateContextAPIObjects(context, combined_apis, highest_api_versions, combined_features, debugger)) {
        TL_Error(debugger, "Failed to create API objects for context at %p in call to TL_CreateRenderers", context);
        return false;
    }

    TL_Note(debugger, "Allocated and populated context data for %d renderers", count);

    // TODO: create each renderer
    //  ...

    context->renderers_init = true;

    return true;
}

/**
 * @brief Free the given renderer object.
 *
 * This function frees the specified renderer object.
 *
 * @param renderer Pointer to the renderer object to free.
 */
void TL_DestroyRenderer(TL_Renderer_t *const renderer) {
    // TODO: destroy renderer
}