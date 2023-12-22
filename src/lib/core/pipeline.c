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

#include <stdlib.h>

TL_Pipeline_t *TL_PipelineCreate(const TL_Renderer_t *const renderer, const TL_PipelineDescriptor_t descriptor) {
    if (!renderer) {
        return NULL;
    }

    const TL_Debugger_t *debugger = renderer->debugger;

    TL_Pipeline_t *pipeline = malloc(sizeof(TL_Pipeline_t));
    if (!pipeline) {
        TL_Fatal(debugger, "MALLOC fault in call to TL_PipelineCreate");
        return NULL;
    }

    TL_Log(debugger, "Allocated pipeline at %p", pipeline);

    return pipeline;
}

void TL_PipelineDestroy(TL_Pipeline_t *const pipeline) {
    // if (!pipeline) {
    //     return;
    // }

    free(pipeline);
}
