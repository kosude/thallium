/*
 *   Copyright (c) 2023 Jack Bennett
 *   All rights reserved.
 *
 *   Please see the LICENCE file for more information.
 */

#include "log.h"

#include "thallium/debug.h"

#include "io_colour.h"
#include "proc.h"
#include "state/state.h"

#include <stdio.h>
#include <stdarg.h>

/**
 * @brief Maximum length of debug messages.
 */
#define MAX_DBGMSG_LEN 512

/**
 * @brief Return from the parent function if the given severity is not included in the user filter.
 */
#define ASSERT_FILTER(sev) \
{ \
    if ((thallium.debugSeverityFilter & sev) != sev) { \
        return; \
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

const void th_Log(const char *format, ...) {
    ASSERT_FILTER(THALLIUM_DEBUG_SEVERITY_VERBOSE_BIT);

    char msg[MAX_DBGMSG_LEN];
    FORMAT_STR(format);

    printf("(thallium!) %s\n", msg);
}

const void th_Note(const char *format, ...) {
    ASSERT_FILTER(THALLIUM_DEBUG_SEVERITY_NOTIF_BIT);

    char msg[MAX_DBGMSG_LEN];
    FORMAT_STR(format);

    th_SetIOColour(THALLIUM_IO_COLOUR_BLUE, 0xFF, stdout);
    printf("(thallium!) %s\n", msg);
    th_DefaultIOColour(stdout);
}

const void th_Warn(const char *format, ...) {
    ASSERT_FILTER(THALLIUM_DEBUG_SEVERITY_WARNING_BIT);

    char msg[MAX_DBGMSG_LEN];
    FORMAT_STR(format);

    th_SetIOColour(THALLIUM_IO_COLOUR_YELLOW, 0xFF, stdout);
    printf("(thallium!) WARN: %s\n", msg);
    th_DefaultIOColour(stdout);
}

const void th_Error(const char *format, ...) {
    ASSERT_FILTER(THALLIUM_DEBUG_SEVERITY_ERROR_BIT);

    char msg[MAX_DBGMSG_LEN];
    FORMAT_STR(format);

    th_SetIOColour(THALLIUM_IO_COLOUR_RED, 0xFF, stderr);
    fprintf(stderr, "(thallium!) ERROR: %s\n", msg);
    th_DefaultIOColour(stderr);
}

const void th_Fatal(const char *format, ...) {
    ASSERT_FILTER(THALLIUM_DEBUG_SEVERITY_FATAL_BIT);

    char msg[MAX_DBGMSG_LEN];
    FORMAT_STR(format);

    th_SetIOColour(THALLIUM_IO_COLOUR_RED, THALLIUM_IO_COLOUR_YELLOW, stderr);
    fprintf(stderr, "(thallium!) FATAL: %s\n", msg);
    th_DefaultIOColour(stderr);

    th_KillProc();
}
