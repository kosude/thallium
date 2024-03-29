/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#include "thallium/core/swapchain.h"
#include "types/core/swapchain_t.h"

#include "types/core/renderer_t.h"
#include "utils/utils.h"

#include "api_modules.h"

#include <stdlib.h>

TL_Swapchain_t *TL_SwapchainCreate(const TL_Renderer_t *const renderer, const TL_SwapchainDescriptor_t descriptor) {
    if (!renderer) {
        return NULL;
    }

    const TL_Debugger_t *debugger = renderer->debugger;

    TL_RendererAPIFlags_t api = renderer->api;
    TL_RendererFeatures_t features = renderer->features;

    // if this bool is true then we assume swapchain creation functions are available for whatever API the renderer is using
    if (!features.presentation) {
        TL_Error(debugger, "Failed to create swapchain: missing renderer feature 'presentation'");
        return NULL;
    }

    TL_Swapchain_t *swapchain = malloc(sizeof(TL_Swapchain_t));
    if (!swapchain) {
        TL_Fatal(debugger, "MALLOC fault in call to TL_SwapchainCreate");
        return NULL;
    }

    TL_Log(debugger, "Allocated swapchain at %p", swapchain);

    swapchain->renderer = renderer;

    // creating API-appropriate swapchain system
    switch (api) {

        // create a Vulkan swapchain system...
        case TL_RENDERER_API_VULKAN_BIT:;
#           if defined(_THALLIUM_VULKAN_INCL)

                TLVK_SwapchainSystemDescriptor_t ssdescr;

                if (descriptor.swapchain_system_descriptor) {
                    ssdescr = *((TLVK_SwapchainSystemDescriptor_t *) descriptor.swapchain_system_descriptor);

                    // fallback to descriptor.resolution
                    if (ssdescr.resolution.width <= 0 || ssdescr.resolution.height <= 0) {
                        ssdescr.resolution = descriptor.resolution;
                    }
                } else {
                    // default swapchain system descriptor configuration (used if no user-given descriptor was specified)...

                    ssdescr.resolution = descriptor.resolution;

                    ssdescr.vk_surface = NULL; // create a surface for descriptor.window_surface (passed to TLVK_SwapchainSystemCreate call).

                    // automatically select properties
                    ssdescr.vk_surface_format.format = -1;
                    ssdescr.vk_present_mode = -1;
                }

                void *renderersys = renderer->renderer_system;

                TLVK_SwapchainSystem_t *swapchainsys = TLVK_SwapchainSystemCreate(renderersys, ssdescr, descriptor.window_surface);
                if (!swapchainsys) {
                    TL_Error(debugger, "Failed to create Vulkan swapchain system for new swapchain at %p", swapchain);
                    return NULL;
                }

                swapchain->swapchain_system = (void *) swapchainsys;
                swapchain->extent = TLVK_SwapchainSystemGetExtent(swapchainsys);

#           endif
            break;

        case TL_RENDERER_API_NULL_BIT:
        default:
            return NULL;

    }

    TL_Note(debugger, "Swapchain %p created for use by renderer %p", swapchain, renderer);

    return swapchain;
}

void TL_SwapchainDestroy(TL_Swapchain_t *const swapchain) {
    if (!swapchain) {
        return;
    }

    TL_RendererAPIFlags_t api = swapchain->renderer->api;

    switch (api) {
        // destroy Vulkan swapchain system
        case TL_RENDERER_API_VULKAN_BIT:
#           if defined(_THALLIUM_VULKAN_INCL)
                TLVK_SwapchainSystemDestroy((TLVK_SwapchainSystem_t *) swapchain->swapchain_system);
#           endif
            break;

        case TL_RENDERER_API_NULL_BIT:
        default:
            break;
    }

    free(swapchain);
}

TL_Extent2D_t TL_SwapchainGetExtent(TL_Swapchain_t *const swapchain) {
    return swapchain->extent;
}
