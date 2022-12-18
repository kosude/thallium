/*
 *   Copyright (c) 2022 Jack Bennett
 *   All rights reserved.
 *
 *   Please see the LICENCE file for more information.
 */

#include "log.h"

#include "io_colour.h"

#include <stdio.h>
#include <stdarg.h>

/**
 * @brief Maximum length of debug messages
 */
#define MAX_DBGMSG_LEN 512

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

void th_Log(const char *format, ...) {
    char msg[MAX_DBGMSG_LEN];
    FORMAT_STR(format);

    printf("(thallium!) %s\n", msg);
}

void th_Note(const char *format, ...) {
    char msg[MAX_DBGMSG_LEN];
    FORMAT_STR(format);

    th_SetIOColour(THALLIUM_IO_COLOUR_BLUE, 0xFF, stdout);
    printf("(thallium!) %s\n", msg);
    th_DefaultIOColour(stdout);
}

void th_Warn(const char *format, ...) {
    char msg[MAX_DBGMSG_LEN];
    FORMAT_STR(format);

    th_SetIOColour(THALLIUM_IO_COLOUR_YELLOW, 0xFF, stdout);
    printf("(thallium!) WARN: %s\n", msg);
    th_DefaultIOColour(stdout);
}

void th_Error(const char *format, ...) {
    char msg[MAX_DBGMSG_LEN];
    FORMAT_STR(format);

    th_SetIOColour(THALLIUM_IO_COLOUR_RED, 0xFF, stderr);
    fprintf(stderr, "(thallium!) ERROR: %s\n", msg);
    th_DefaultIOColour(stderr);
}

void th_Fatal(const char *format, ...) {
    char msg[MAX_DBGMSG_LEN];
    FORMAT_STR(format);

    th_SetIOColour(THALLIUM_IO_COLOUR_RED, THALLIUM_IO_COLOUR_YELLOW, stderr);
    fprintf(stderr, "(thallium!) FATAL: %s\n", msg);
    th_DefaultIOColour(stderr);
}
