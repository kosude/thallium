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

TLVK_RenderSystem_t *TLVK_CreateRenderSystem(const TL_Renderer_t *const renderer, const TLVK_RenderSystemDescriptor_t descriptor) {
    // if renderer is not NULL then we can assume the context was populated, as renderers are only created in core after populating their context
    if (!renderer) {
        return NULL;
    }

    TLVK_RenderSystem_t *render_system = malloc(sizeof(TLVK_RenderSystem_t));
    if (!render_system) {
        TL_Fatal(renderer->debugger, "MALLOC fault in call to TLVK_CreateRenderSystem");
        return NULL;
    }

    // stored on stack for less derefs
    const TL_Debugger_t *debugger = renderer->debugger;

    TL_Log(debugger, "Allocated memory for Vulkan render system at %p", render_system);

    render_system->renderer = renderer;
    render_system->vk_context = (TLVK_ContextVulkanBlock_t *) ((char *) renderer->context->data + renderer->context->vulkan_offset);

    // TODO: create device manager for Vulkan devices (descriptor should have a device manager descriptor)

    return render_system;
}

void TLVK_DestroyRenderSystem(TLVK_RenderSystem_t *const render_system) {
    if (!render_system) {
        return;
    }

    free(render_system);
}
