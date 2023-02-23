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

const int th_DestroyDebugger(th_Debugger_t *debugger) {
    free(debugger);

    return 1;
}

const void th_TriggerSeverityMessages(const th_Debugger_t *debugger) {
    th_Log(debugger, "Enabled: core library logging + verbose messages");
    th_Log_Vk(debugger, "Enabled: Vulkan debug messages (sent by Vulkan renderers)");
    th_Note(debugger, "Enabled: notification messages");
    th_Hint(debugger, "Enabled: hints (usually to further describe other messages)");
    th_Warn(debugger, "Enabled: core library warnings");
    th_Warn_Vk(debugger, "Enabled: Vulkan warnings (sent by Vulkan renderers)");
    th_Error(debugger, "Enabled: core library errors");
    th_Error_Vk(debugger, "Enabled: Vulkan errors (sent by Vulkan renderers)");
    th_Fatal(debugger, "Enabled: fatal errors");
}
