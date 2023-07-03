/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#include "thallium_vulkan.h"

#include "utils/io/log.h"

#include "types/vulkan/vk_render_system.h"
#include "types/core/context.h"
#include "types/core/renderer.h"

#include <stdlib.h>

TLVK_RenderSystem_t *TLVK_RenderSystemCreate(const TL_Renderer_t *const renderer, const TLVK_RenderSystemDescriptor_t descriptor) {
    // if renderer is not NULL then we can assume the context was populated, as renderers are only created in core after populating their context
    if (!renderer) {
        return NULL;
    }

    const TL_Debugger_t *debugger = renderer->debugger;

    TLVK_RenderSystem_t *render_system = malloc(sizeof(TLVK_RenderSystem_t));
    if (!render_system) {
        TL_Fatal(debugger, "MALLOC fault in call to TLVK_RenderSystemCreate");
        return NULL;
    }

    TL_Log(debugger, "Allocated memory for Vulkan render system at %p", render_system);

    render_system->renderer = renderer;
    render_system->vk_context = (TLVK_ContextBlock_t *) ((char *) renderer->context->data + renderer->context->vulkan_offset);
    render_system->features = renderer->features;

    // populate descriptor for device manager
    TLVK_DeviceManagerDescriptor_t device_manager_descriptor;
    if (descriptor.device_manager_descriptor) {
        device_manager_descriptor = *(descriptor.device_manager_descriptor);
    } else {
        // default device manager descriptor configuration (used if no user-given descriptor was specified)...

        // TODO device manager descriptor
        device_manager_descriptor.placeholder = 5;
    }

    render_system->device_manager = TLVK_DeviceManagerCreate(render_system, device_manager_descriptor);
    if (!render_system->device_manager) {
        TL_Error(debugger, "Failed to create Vulkan device manager for render system at %p", render_system);
        return NULL;
    }

    return render_system;
}

void TLVK_RenderSystemDestroy(TLVK_RenderSystem_t *const render_system) {
    if (!render_system) {
        return;
    }

    TLVK_DeviceManagerDestroy(render_system->device_manager);

    free(render_system);
}
