/*
 *   Copyright (c) 2023 Jack Bennett
 *   All rights reserved.
 *
 *   Please see the LICENCE file for more information.
 */

#include "log.h"

#include "thallium/core/debugger.h"

#include "utils/io_colour.h"
#include "utils/proc.h"
#include "types.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

// String to add to Thallium debug messages as a prefix
#define THALLIUM_DBGMSG_PREFIX "(thallium!)"

// Maximum length of debug messages.
#define MAX_DBGMSG_LEN 512

// Return from the parent function if the given severity is not included in the debugger filter.
#define ASSERT_SEV_FILTER(debugger, sev) \
{ \
    if (!debugger || (debugger->debugSeverityFilter & sev) != sev) { \
        return 1; \
    } \
}

// Return from the parent function if the given message type is not included in the debugger filter.
#define ASSERT_TYPE_FILTER(debugger, sev) \
{ \
    if (!debugger || (debugger->debugTypeFilter & sev) != sev) { \
        return 1; \
    } \
}

// Format the string and output it into a local variable called `msg`.
#define FORMAT_STR(format) \
{ \
    va_list varargs; \
    va_start(varargs, format); \
    /* format message string with given format + variadic arguments */ \
    vsnprintf(msg, MAX_DBGMSG_LEN, format, varargs); \
    va_end(varargs); \
}

// Helper macro to print log messages with given prefix
#define PRINT_LOG_MSG(debugger, format, prefix) \
{ \
    ASSERT_SEV_FILTER(debugger, THALLIUM_DEBUG_SEVERITY_VERBOSE_BIT); \
    char msg[MAX_DBGMSG_LEN]; \
    FORMAT_STR(format); \
    printf("%s %s\n", prefix, msg); \
}

// Helper macro to print notif messages with given prefix
#define PRINT_NOTE_MSG(debugger, format, prefix) \
{ \
    ASSERT_SEV_FILTER(debugger, THALLIUM_DEBUG_SEVERITY_NOTIF_BIT); \
    char msg[MAX_DBGMSG_LEN]; \
    FORMAT_STR(format); \
    th_SetIOColour(THALLIUM_IO_COLOUR_BLUE, 0xFF, stdout); \
    printf("%s %s\n", prefix, msg); \
    th_DefaultIOColour(stdout); \
}

// Helper macro to print hints with given prefix
#define PRINT_HINT_MSG(debugger, format, prefix) \
{ \
    ASSERT_SEV_FILTER(debugger, THALLIUM_DEBUG_SEVERITY_NOTIF_BIT); \
    char msg[MAX_DBGMSG_LEN]; \
    FORMAT_STR(format); \
    th_SetIOColour(THALLIUM_IO_COLOUR_GREY, 0xFF, stdout); \
    printf("%s Hint: %s\n", prefix, msg); \
    th_DefaultIOColour(stdout); \
}

// Helper macro to print warnings with given prefix
#define PRINT_WARNING_MSG(debugger, format, prefix) \
{ \
    ASSERT_SEV_FILTER(debugger, THALLIUM_DEBUG_SEVERITY_WARNING_BIT); \
    char msg[MAX_DBGMSG_LEN]; \
    FORMAT_STR(format); \
    th_SetIOColour(THALLIUM_IO_COLOUR_YELLOW, 0xFF, stdout); \
    printf("%s WARN: %s\n", prefix, msg); \
    th_DefaultIOColour(stdout); \
}

// Helper macro to print errors with given prefix
#define PRINT_ERROR_MSG(debugger, format, prefix) \
{ \
    ASSERT_SEV_FILTER(debugger, THALLIUM_DEBUG_SEVERITY_ERROR_BIT); \
    char msg[MAX_DBGMSG_LEN]; \
    FORMAT_STR(format); \
    th_SetIOColour(THALLIUM_IO_COLOUR_RED, 0xFF, stderr); \
    fprintf(stderr, "%s ERROR: %s\n", prefix, msg); \
    th_DefaultIOColour(stderr); \
}

// Helper macro to print fatal errors with given prefix
#define PRINT_FATAL_MSG(debugger, format, prefix) \
{ \
    ASSERT_SEV_FILTER(debugger, THALLIUM_DEBUG_SEVERITY_FATAL_BIT); \
    char msg[MAX_DBGMSG_LEN]; \
    FORMAT_STR(format); \
    th_SetIOColour(THALLIUM_IO_COLOUR_RED, THALLIUM_IO_COLOUR_YELLOW, stderr); \
    fprintf(stderr, "%s FATAL: %s\n", prefix, msg); \
    th_DefaultIOColour(stderr); \
}

// Macro to define debug logging functions
#define DEBUG_MSG_FUN(functionName, printFunction, prefix, killProc) \
    const int functionName(const th_Debugger_t *debugger, const char *format, ...)  { \
        /* If function contains _Vk then we are sending a Vulkan debug message. */ \
        if (strstr(#functionName, "_Vk") != NULL) { ASSERT_TYPE_FILTER(debugger, THALLIUM_DEBUG_TYPE_VULKAN_BIT); } \
        /* Otherwise we are sending a Thallium debug message */ \
        else { ASSERT_TYPE_FILTER(debugger, THALLIUM_DEBUG_TYPE_CORE_BIT); } \
\
        printFunction(debugger, format, prefix); \
\
        if (killProc) th_KillProc(); \
\
        return 1; \
    } \

//
// DEFINE DEBUG MESSAGE LOGGING FUNCTIONS
//

DEBUG_MSG_FUN(th_Log, PRINT_LOG_MSG, "(th)", 0)
DEBUG_MSG_FUN(th_Log_Vk, PRINT_LOG_MSG, "(thvk)", 0)

DEBUG_MSG_FUN(th_Note, PRINT_NOTE_MSG, "(th)", 0)

DEBUG_MSG_FUN(th_Hint, PRINT_HINT_MSG, "(th)", 0)

DEBUG_MSG_FUN(th_Warn, PRINT_WARNING_MSG, "(th)", 0)
DEBUG_MSG_FUN(th_Warn_Vk, PRINT_WARNING_MSG, "(thvk)", 0)

DEBUG_MSG_FUN(th_Error, PRINT_ERROR_MSG, "(th)", 0)
DEBUG_MSG_FUN(th_Error_Vk, PRINT_ERROR_MSG, "(thvk)", 0)

DEBUG_MSG_FUN(th_Fatal, PRINT_FATAL_MSG, "(th)", 1)
