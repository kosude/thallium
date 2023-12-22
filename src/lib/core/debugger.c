/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#include "thallium/core/debugger.h"
#include "types/core/debugger.h"

#include "utils/utils.h"

#include <stdlib.h>

TL_Debugger_t *TL_DebuggerCreate(const TL_DebuggerDescriptor_t descriptor) {
    TL_Debugger_t *debugger = malloc(sizeof(TL_Debugger_t));
    if (!debugger) {
        return NULL;
    }

    debugger->severities = descriptor.severities;
    debugger->sources = descriptor.sources;
    debugger->user_callback = descriptor.callback;
    debugger->user_pointer = descriptor.pointer;

    return debugger;
}

void TL_DebuggerDestroy(TL_Debugger_t *const debugger) {
    if (!debugger) {
        return;
    }

    free(debugger);
}
