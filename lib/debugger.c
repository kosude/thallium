/*
 *   Copyright (c) 2023 Jack Bennett
 *   All rights reserved.
 *
 *   Please see the LICENCE file for more information.
 */

#include "thallium/core/debugger.h"

#include "utils/log.h"
#include "types.h"

#include <stdlib.h>
#include <stdio.h>
#include "assert.h"


// ===========================================================================
//                       THALLIUM PUBLIC API DEFINITIONS
// ===========================================================================

th_Debugger_t *th_CreateDebugger(const th_DebugSeverity_t severities, const th_DebugType_t types) {
    // note that in this function, printf is used instead of log functions sbecause we
    // don't yet have a debugger to pass as argument

#   ifndef THALLIUM_DEBUG_LAYER
        return NULL;
#   endif

    th_Debugger_t *r = malloc(sizeof(th_Debugger_t));
    if (!r) {
        printf("THALLIUM PRE-DEBUGGER FATAL: MALLOC fault in th_CreateDebugger!\n");
        return NULL;
    }

    r->debug_severity_filter = severities;
    r->debug_type_filter = types;

    return r;
}

int th_DestroyDebugger(th_Debugger_t *debugger) {
    free(debugger);

    return 1;
}
