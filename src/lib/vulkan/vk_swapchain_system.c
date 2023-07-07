/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#include "thallium_vulkan.h"

#include "utils/utils.h"

#include "types/core/renderer.h"
#include "types/vulkan/vk_renderer_system.h"
#include "types/vulkan/vk_swapchain_system.h"

#include <stdlib.h>

TLVK_SwapchainSystem_t *TLVK_SwapchainSystemCreate(const TLVK_RendererSystem_t *const renderer_system,
    const TLVK_SwapchainSystemDescriptor_t descriptor)
{
    if (!renderer_system) {
        return NULL;
    }

    const TL_Debugger_t *debugger = renderer_system->renderer->debugger;

    TLVK_SwapchainSystem_t *swapchain_system = malloc(sizeof(TLVK_SwapchainSystem_t));
    if (!swapchain_system) {
        TL_Fatal(debugger, "MALLOC fault in call to TLVK_SwapchainSystemCreate");
        return NULL;
    }

    TL_Log(debugger, "Allocated memory for Vulkan swapchain system at %p", swapchain_system);

    return swapchain_system;
}

void TLVK_SwapchainSystemDestroy(TLVK_SwapchainSystem_t *const swapchain_system) {
    if (!swapchain_system) {
        return;
    }

    free(swapchain_system);
}
