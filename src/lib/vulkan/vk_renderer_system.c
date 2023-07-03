/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#include "thallium_vulkan.h"

#include "utils/io/log.h"

#include "types/vulkan/vk_renderer_system.h"
#include "types/core/context.h"
#include "types/core/renderer.h"

#include <stdlib.h>

TLVK_RendererSystem_t *TLVK_RendererSystemCreate(const TL_Renderer_t *const renderer, const TLVK_RendererSystemDescriptor_t descriptor) {
    // if renderer is not NULL then we can assume the context was populated, as renderers are only created in core after populating their context
    if (!renderer) {
        return NULL;
    }

    const TL_Debugger_t *debugger = renderer->debugger;

    TLVK_RendererSystem_t *renderer_system = malloc(sizeof(TLVK_RendererSystem_t));
    if (!renderer_system) {
        TL_Fatal(debugger, "MALLOC fault in call to TLVK_RendererSystemCreate");
        return NULL;
    }

    TL_Log(debugger, "Allocated memory for Vulkan renderer system at %p", renderer_system);

    renderer_system->renderer = renderer;
    renderer_system->vk_context = (TLVK_ContextBlock_t *) ((char *) renderer->context->data + renderer->context->vulkan_offset);
    renderer_system->features = renderer->features;

    // populate descriptor for device manager
    TLVK_DeviceManagerDescriptor_t device_manager_descriptor;
    if (descriptor.device_manager_descriptor) {
        device_manager_descriptor = *(descriptor.device_manager_descriptor);
    } else {
        // default device manager descriptor configuration (used if no user-given descriptor was specified)...

        // TODO device manager descriptor
        device_manager_descriptor.placeholder = 5;
    }

    renderer_system->device_manager = TLVK_DeviceManagerCreate(renderer_system, device_manager_descriptor);
    if (!renderer_system->device_manager) {
        TL_Error(debugger, "Failed to create Vulkan device manager for renderer system at %p", renderer_system);
        return NULL;
    }

    return renderer_system;
}

void TLVK_RendererSystemDestroy(TLVK_RendererSystem_t *const renderer_system) {
    if (!renderer_system) {
        return;
    }

    TLVK_DeviceManagerDestroy(renderer_system->device_manager);

    free(renderer_system);
}
