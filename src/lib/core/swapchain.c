/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#include "thallium.h"
#include "api_modules.h"

#include "types/core/renderer.h"
#include "types/core/swapchain.h"

#include "utils/utils.h"

#include <stdlib.h>

TL_Swapchain_t *TL_SwapchainCreate(const TL_Renderer_t *const renderer, const TL_SwapchainDescriptor_t descriptor) {
    if (!renderer) {
        return NULL;
    }

    const TL_Debugger_t *debugger = renderer->debugger;

    // if this bool is true then we assume swapchain creation functions are available for whatever API the renderer is using
    if (!renderer->features.presentation) {
        TL_Error(debugger, "Failed to create swapchain: missing renderer feature 'presentation'");
        return NULL;
    }

    TL_Swapchain_t *swapchain = malloc(sizeof(TL_Swapchain_t));
    if (!renderer) {
        TL_Fatal(debugger, "MALLOC fault in call to TL_SwapchainCreate");
        return NULL;
    }

    TL_Log(renderer->debugger, "Allocated swapchain at %p", swapchain);

    swapchain->renderer = renderer;

    // creating API-appropriate swapchain system
    switch (renderer->api) {

        // create a Vulkan swapchain system...
        case TL_RENDERER_API_VULKAN_BIT:; // the semicolon somehow fixes variable declaration errors (fml)
#           if defined(_THALLIUM_VULKAN_INCL)

                TLVK_SwapchainSystemDescriptor_t ssdescr;

                if (descriptor.swapchain_system_descriptor) {
                    ssdescr = *((TLVK_SwapchainSystemDescriptor_t *) descriptor.swapchain_system_descriptor);
                } else {
                    // default swapchain system descriptor configuration (used if no user-given descriptor was specified)...

                    ssdescr.placeholder = 5;
                }

                swapchain->swapchain_system = (void *) TLVK_SwapchainSystemCreate(renderer->renderer_system, ssdescr);
                if (!swapchain->swapchain_system) {
                    TL_Error(debugger, "Failed to create Vulkan swapchain system for new swapchain at %p", swapchain);
                    return NULL;
                }

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

    free(swapchain);
}
