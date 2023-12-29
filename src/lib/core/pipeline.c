/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#include "thallium/core/pipeline.h"
#include "types/core/pipeline_t.h"

#include "types/core/renderer_t.h"
#include "utils/io/log.h"

#include "api_modules.h"

#include <stdlib.h>

TL_Pipeline_t *TL_PipelineCreate(const TL_Renderer_t *const renderer, const TL_PipelineDescriptor_t descriptor) {
    if (!renderer) {
        return NULL;
    }

    const TL_Debugger_t *debugger = renderer->debugger;

    TL_RendererAPIFlags_t api = renderer->api;

    TL_Pipeline_t *pipeline = malloc(sizeof(TL_Pipeline_t));
    if (!pipeline) {
        TL_Fatal(debugger, "MALLOC fault in call to TL_PipelineCreate");
        return NULL;
    }

    TL_Log(debugger, "Allocated pipeline at %p", pipeline);

    // creating API-appropriate pipeline system
    switch (api) {

        // create a Vulkan pipeline system...
        case TL_RENDERER_API_VULKAN_BIT:;
#           if defined(_THALLIUM_VULKAN_INCL)

                void *renderersys = renderer->renderer_system;

                TLVK_PipelineSystem_t *pipelinesys = TLVK_PipelineSystemCreate(renderersys, descriptor);
                if (!pipelinesys) {
                    TL_Error(debugger, "Failed to create Vulkan pipeline system for new pipeline at %p", pipeline);
                    return NULL;
                }

                pipeline->pipeline_system = (void *) pipelinesys;

#           endif
            break;

        case TL_RENDERER_API_NULL_BIT:
        default:
            return NULL;

    }

    return pipeline;
}

void TL_PipelineDestroy(TL_Pipeline_t *const pipeline) {
    if (!pipeline) {
        return;
    }

    TL_RendererAPIFlags_t api = pipeline->renderer->api;

    switch (api) {
        // destroy Vulkan pipeline system
        case TL_RENDERER_API_VULKAN_BIT:
#           if defined(_THALLIUM_VULKAN_INCL)
                TLVK_PipelineSystemDestroy((TLVK_PipelineSystem_t *) pipeline->pipeline_system);
#           endif
            break;

        case TL_RENDERER_API_NULL_BIT:
        default:
            break;
    }

    free(pipeline);
}
