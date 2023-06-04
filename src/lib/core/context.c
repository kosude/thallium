/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#include "types/core/context.h"

#include "utils/utils.h"

#include <stdlib.h>

// static context pointer singleton
static TL_Context_t *__CONTEXT_PTR = NULL;

TL_Context_t *TL_CreateContext(const TL_Debugger_t *const debugger) {
    if (__CONTEXT_PTR) {
        TL_Warn(debugger, "Attempted to create context (one already exists at %p) - creating multiple contexts is illegal. Existing context was "
            "returned", __CONTEXT_PTR);

        return __CONTEXT_PTR;
    }

    TL_Context_t *context = malloc(sizeof(TL_Context_t));
    if (!context) {
        TL_Fatal(debugger, "MALLOC fault in call to TL_CreateContext");
        return NULL;
    }

    TL_Note(debugger, "Allocated blank context at %p", context);

    // SIZE_MAX for these members means the data block is not initialised
    // the handles of any data blocks that are initialised will be set accordingly
    context->vulkan_offset = SIZE_MAX;

    // initialise to 0 as the data depends on which APIs are used.
    context->data_size = 0;
    context->data = NULL;

    __CONTEXT_PTR = context;

    return context;
}

void TL_DestroyContext(TL_Context_t *const context) {
    if (!context) {
        return;
    }

    free(context);
}
