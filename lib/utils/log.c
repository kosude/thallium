/*
 *   Copyright (c) 2023 Jack Bennett
 *   All rights reserved.
 *
 *   Please see the LICENCE file for more information.
 */

#include "log.h"

#include "thallium/debug.h"

#include "utils/io_colour.h"
#include "utils/proc.h"
#include "types.h"

#include <stdio.h>
#include <stdarg.h>

/**
 * @brief String to add to Thallium debug messages as a prefix
 */
#define THALLIUM_DBGMSG_PREFIX "(thallium!)"

/**
 * @brief Maximum length of debug messages.
 */
#define MAX_DBGMSG_LEN 512

/**
 * @brief Return from the parent function if the given severity is not included in the debugger filter.
 */
#define ASSERT_FILTER(debugger, sev) \
{ \
    if (!debugger || (debugger->debugSeverityFilter & sev) != sev) { \
        return 1; \
    } \
}

/**
 * @brief Format the string and output it into a local variable called `msg`.
 */
#define FORMAT_STR(format) \
{ \
    va_list varargs; \
    va_start(varargs, format); \
\
    /* format message string with given format + variadic arguments */ \
    vsnprintf(msg, MAX_DBGMSG_LEN, format, varargs); \
\
    va_end(varargs); \
}

const int th_Log(const th_Debugger_t *debugger, const char *format, ...) {
#   if defined(THALLIUM_DEBUG_LAYER)
        ASSERT_FILTER(debugger, THALLIUM_DEBUG_SEVERITY_VERBOSE_BIT);

        char msg[MAX_DBGMSG_LEN];
        FORMAT_STR(format);

        printf("%s %s\n", THALLIUM_DBGMSG_PREFIX, msg);
#   endif

    return 1;
}

const int th_Note(const th_Debugger_t *debugger, const char *format, ...) {
#   if defined(THALLIUM_DEBUG_LAYER)
        ASSERT_FILTER(debugger, THALLIUM_DEBUG_SEVERITY_NOTIF_BIT);

        char msg[MAX_DBGMSG_LEN];
        FORMAT_STR(format);

        th_SetIOColour(THALLIUM_IO_COLOUR_BLUE, 0xFF, stdout);
        printf("%s %s\n", THALLIUM_DBGMSG_PREFIX, msg);
        th_DefaultIOColour(stdout);
#   endif

    return 1;
}

const int th_Hint(const th_Debugger_t *debugger, const char *format, ...) {
#   if defined(THALLIUM_DEBUG_LAYER)
        ASSERT_FILTER(debugger, THALLIUM_DEBUG_SEVERITY_NOTIF_BIT);

        char msg[MAX_DBGMSG_LEN];
        FORMAT_STR(format);

        th_SetIOColour(THALLIUM_IO_COLOUR_GREY, 0xFF, stdout);
        printf("%s Hint: %s\n", THALLIUM_DBGMSG_PREFIX, msg);
        th_DefaultIOColour(stdout);
#   endif

    return 1;
}

const int th_Warn(const th_Debugger_t *debugger, const char *format, ...) {
#   if defined(THALLIUM_DEBUG_LAYER)
        ASSERT_FILTER(debugger, THALLIUM_DEBUG_SEVERITY_WARNING_BIT);

        char msg[MAX_DBGMSG_LEN];
        FORMAT_STR(format);

        th_SetIOColour(THALLIUM_IO_COLOUR_YELLOW, 0xFF, stdout);
        printf("%s WARN: %s\n", THALLIUM_DBGMSG_PREFIX, msg);
        th_DefaultIOColour(stdout);
#   endif

    return 1;
}

const int th_Error(const th_Debugger_t *debugger, const char *format, ...) {
#   if defined(THALLIUM_DEBUG_LAYER)
        ASSERT_FILTER(debugger, THALLIUM_DEBUG_SEVERITY_ERROR_BIT);

        char msg[MAX_DBGMSG_LEN];
        FORMAT_STR(format);

        th_SetIOColour(THALLIUM_IO_COLOUR_RED, 0xFF, stderr);
        fprintf(stderr, "%s ERROR: %s\n", THALLIUM_DBGMSG_PREFIX, msg);
        th_DefaultIOColour(stderr);
#   endif

    return 1;
}

const int th_Fatal(const th_Debugger_t *debugger, const char *format, ...) {
#   if defined(THALLIUM_DEBUG_LAYER)
        ASSERT_FILTER(debugger, THALLIUM_DEBUG_SEVERITY_FATAL_BIT);

        char msg[MAX_DBGMSG_LEN];
        FORMAT_STR(format);

        th_SetIOColour(THALLIUM_IO_COLOUR_RED, THALLIUM_IO_COLOUR_YELLOW, stderr);
        fprintf(stderr, "%s FATAL: %s\n", THALLIUM_DBGMSG_PREFIX, msg);
        th_DefaultIOColour(stderr);
#   endif

    th_KillProc();

    return 1;
}
