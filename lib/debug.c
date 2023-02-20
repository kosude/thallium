/*
 *   Copyright (c) 2023 Jack Bennett
 *   All rights reserved.
 *
 *   Please see the LICENCE file for more information.
 */

#include "thallium/core/debug.h"

#include "types.h"

#include <stdlib.h>
#include <stdio.h>


// ===========================================================================
//                       THALLIUM PUBLIC API DEFINITIONS
// ===========================================================================

th_Debugger_t *th_CreateDebugger(const th_DebugSeverity_t severities) {
    // note that in this function, printf is used instead of log functions sbecause we
    // don't yet have a debugger to pass as argument

#   ifndef THALLIUM_DEBUG_LAYER
        printf("THALLIUM PRE-DEBUGGER WARNING: Created debugger without enabling debug layer... no messages will be shown!\n");
#   endif

    th_Debugger_t *r = malloc(sizeof(th_Debugger_t));
    if (!r) {
        printf("THALLIUM PRE-DEBUGGER FATAL: MALLOC fault in th_CreateDebugger!\n");
        return NULL;
    }

    r->debugSeverityFilter = severities;

    return r;
}

const int th_DestroyDebugger(th_Debugger_t *debugger) {
    free(debugger);

    return 1;
}
