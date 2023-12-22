/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#include "log.h"

#include "utils/io/proc.h"
#include "types/core/debugger_t.h"

#include <stdio.h>
#include <stdarg.h>

// Maximum length of debug messages:
#define __MAX_DBGMSG_LEN 1024


static void __DefaultLogFun(char *msg, TL_DebugSeverityFlags_t sev);


void TL_ReportMessage(const TL_Debugger_t *const debugger, const TL_DebugSeverityFlags_t severity, const TL_DebugSourceFlags_t source,
    const char *const format, ...)
{
    // check against severity
    if (!debugger || !(debugger->severities & severity)) {
        return;
    }

    // check against source
    if (!debugger || !(debugger->sources & source)) {
        return;
    }

    char msg[__MAX_DBGMSG_LEN];

    // format string msg with given format + variadic arguments
    va_list varargs;
    va_start(varargs, format);
    vsnprintf(msg, __MAX_DBGMSG_LEN, format, varargs);
    va_end(varargs);

    TL_DebugCallbackfn_t cbfun = debugger->user_callback;

    if (cbfun) {
        // invoke set user callback
        cbfun(
            msg,
            severity,
            source,
            debugger->user_pointer);
    } else {
        // if no user callback is set, we use default behaviour
        __DefaultLogFun(msg, severity);
    }

    if (severity & TL_DEBUG_SEVERITY_FATAL_BIT) {
        TL_KillProc();
    }
}


static void __DefaultLogFun(char *msg, TL_DebugSeverityFlags_t sev) {
    FILE *out = stderr;

    switch (sev) {
        case TL_DEBUG_SEVERITY_VERBOSE_BIT:
            fprintf(out, "(log) ");
            break;
        case TL_DEBUG_SEVERITY_NOTIF_BIT:
            fprintf(out, "(note) ");
            break;
        case TL_DEBUG_SEVERITY_WARNING_BIT:
            fprintf(out, "(warn) ");
            break;
        case TL_DEBUG_SEVERITY_ERROR_BIT:
            fprintf(out, "(error) ");
            break;
        case TL_DEBUG_SEVERITY_FATAL_BIT:
            fprintf(out, "(fatal) ");
            break;
        default:
            break;
    }

    fprintf(out, "%s\n", msg);
}
