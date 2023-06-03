/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#include "log.h"

#include "thallium/platform.h"
#include "thallium/core/debugger.h"

#include "core/utils/proc.h"
#include "types_core.h"

#include <cutils/cio/cio.h>
#include <cutils/cio/cioenum.h>

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#define CORELIB_MSG_PREFIX "(TL)"
#define VULKAN_MSG_PREFIX "(TLVK)"

// Maximum length of debug messages.
#define MAX_DBGMSG_LEN 1024

// Return from the parent function if the given severity is not included in the debugger filter.
#define ASSERT_SEV_FILTER(debugger, sev)                                    \
{                                                                           \
    if (!debugger || (debugger->severities & sev) != sev) {                 \
        return;                                                             \
    }                                                                       \
}

// Return from the parent function if the given message type is not included in the debugger filter.
#define ASSERT_TYPE_FILTER(debugger, sev)                                   \
{                                                                           \
    if (!debugger || (debugger->sources & sev) != sev) {                    \
        return;                                                             \
    }                                                                       \
}

// Format the string and output it into a local variable called `msg`.
#define FORMAT_STR(format)                                                  \
{                                                                           \
    va_list varargs;                                                        \
    va_start(varargs, format);                                              \
    /* format message string with given format + variadic arguments */      \
    vsnprintf(msg, MAX_DBGMSG_LEN, format, varargs);                        \
    va_end(varargs);                                                        \
}

// Helper macro to print log messages with given prefix
#define PRINT_LOG_MSG(debugger, format, prefix)                             \
{                                                                           \
    ASSERT_SEV_FILTER(debugger, TL_DEBUG_SEVERITY_VERBOSE_BIT);             \
    char msg[MAX_DBGMSG_LEN];                                               \
    FORMAT_STR(format);                                                     \
    printf("%s %s\n", prefix, msg);                                         \
}

// Helper macro to print notif messages with given prefix
#define PRINT_NOTE_MSG(debugger, format, prefix)                            \
{                                                                           \
    ASSERT_SEV_FILTER(debugger, TL_DEBUG_SEVERITY_NOTIF_BIT);               \
    char msg[MAX_DBGMSG_LEN];                                               \
    FORMAT_STR(format);                                                     \
    ciocolstateset(CIOCOL_BLUE, 0xFF, stdout);                              \
    printf("%s %s\n", prefix, msg);                                         \
    ciocolstatedef(stdout);                                                 \
}

// Helper macro to print hints with given prefix
#define PRINT_HINT_MSG(debugger, format, prefix)                            \
{                                                                           \
    ASSERT_SEV_FILTER(debugger, TL_DEBUG_SEVERITY_NOTIF_BIT);               \
    char msg[MAX_DBGMSG_LEN];                                               \
    FORMAT_STR(format);                                                     \
    ciocolstateset(CIOCOL_GREY, 0xFF, stdout);                              \
    printf("%s Hint: %s\n", prefix, msg);                                   \
    ciocolstatedef(stdout);                                                 \
}

// Helper macro to print warnings with given prefix
#define PRINT_WARNING_MSG(debugger, format, prefix)                         \
{                                                                           \
    ASSERT_SEV_FILTER(debugger, TL_DEBUG_SEVERITY_WARNING_BIT);             \
    char msg[MAX_DBGMSG_LEN];                                               \
    FORMAT_STR(format);                                                     \
    ciocolstateset(CIOCOL_YELLOW, 0xFF, stdout);                            \
    printf("%s WARN: %s\n", prefix, msg);                                   \
    ciocolstatedef(stdout);                                                 \
}

// Helper macro to print errors with given prefix
#define PRINT_ERROR_MSG(debugger, format, prefix)                           \
{                                                                           \
    ASSERT_SEV_FILTER(debugger, TL_DEBUG_SEVERITY_ERROR_BIT);               \
    char msg[MAX_DBGMSG_LEN];                                               \
    FORMAT_STR(format);                                                     \
    ciocolstateset(CIOCOL_RED, 0xFF, stderr);                               \
    fprintf(stderr, "%s ERROR: %s\n", prefix, msg);                         \
    ciocolstatedef(stderr);                                                 \
}

// Helper macro to print fatal errors with given prefix
#define PRINT_FATAL_MSG(debugger, format, prefix)                           \
{                                                                           \
    ASSERT_SEV_FILTER(debugger, TL_DEBUG_SEVERITY_FATAL_BIT);               \
    char msg[MAX_DBGMSG_LEN];                                               \
    FORMAT_STR(format);                                                     \
    ciocolstateset(CIOCOL_RED, 0xFF, stderr);                               \
    fprintf(stderr, "%s FATAL: %s\n", prefix, msg);                         \
    ciocolstatedef(stderr);                                                 \
}

// Macro to define core debug logging functions
#define DEBUG_MSG_FUN(functionName, printFunction, prefix, killProc)                                                        \
    void functionName(const TL_Debugger_t *const debugger, const char *const format, ...) {                                 \
        ASSERT_TYPE_FILTER(debugger, TL_DEBUG_SOURCE_THALLIUM_BIT);                                                         \
        printFunction(debugger, format, prefix);                                                                            \
        if (killProc) TL_KillProc();                                                                                        \
    }

// Macro to define vulkan debug logging functions
#define DEBUG_MSG_FUN_VK(functionName, printFunction, prefix, killProc)                                                     \
    void functionName(const TL_Debugger_t *const debugger, const char *const format, ...) {                                 \
        ASSERT_TYPE_FILTER(debugger, TL_DEBUG_SOURCE_VULKAN_BIT);                                                           \
        printFunction(debugger, format, prefix);                                                                            \
        if (killProc) TL_KillProc();                                                                                        \
    }

DEBUG_MSG_FUN   (TL_Log,           PRINT_LOG_MSG,      CORELIB_MSG_PREFIX,     false)
DEBUG_MSG_FUN_VK(TL_Log_Vk,        PRINT_LOG_MSG,      VULKAN_MSG_PREFIX,      false)

DEBUG_MSG_FUN   (TL_Note,          PRINT_NOTE_MSG,     CORELIB_MSG_PREFIX,     false)

DEBUG_MSG_FUN   (TL_Hint,          PRINT_HINT_MSG,     CORELIB_MSG_PREFIX,     false)

DEBUG_MSG_FUN   (TL_Warn,          PRINT_WARNING_MSG,  CORELIB_MSG_PREFIX,     false)
DEBUG_MSG_FUN_VK(TL_Warn_Vk,       PRINT_WARNING_MSG,  VULKAN_MSG_PREFIX,      false)

DEBUG_MSG_FUN   (TL_Error,         PRINT_ERROR_MSG,    CORELIB_MSG_PREFIX,     false)
DEBUG_MSG_FUN_VK(TL_Error_Vk,      PRINT_ERROR_MSG,    VULKAN_MSG_PREFIX,      false)

DEBUG_MSG_FUN   (TL_Fatal,         PRINT_FATAL_MSG,    CORELIB_MSG_PREFIX,     true)
