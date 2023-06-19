/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#pragma once
#ifndef __TL__internal__utils__log_h__
#define __TL__internal__utils__log_h__
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

#include "thallium/enums.h"
#include "thallium/fwd.h"
#include "thallium/platform.h"

void TL_ReportMessage(
    const TL_Debugger_t *const debugger,
    const TL_DebugSeverityFlags_t severity,
    const TL_DebugSourceFlags_t source,
    const char *const format,
    ...
);

#define TL_Log(debugger, ...)       \
    TL_ReportMessage(debugger, TL_DEBUG_SEVERITY_VERBOSE_BIT, TL_DEBUG_SOURCE_THALLIUM_BIT, __VA_ARGS__)
#define TL_Log_Vk(debugger, ...)    \
    TL_ReportMessage(debugger, TL_DEBUG_SEVERITY_VERBOSE_BIT, TL_DEBUG_SOURCE_VULKAN_BIT, __VA_ARGS__)

#define TL_Note(debugger, ...)      \
    TL_ReportMessage(debugger, TL_DEBUG_SEVERITY_NOTIF_BIT, TL_DEBUG_SOURCE_THALLIUM_BIT, __VA_ARGS__)

#define TL_Warn(debugger, ...)      \
    TL_ReportMessage(debugger, TL_DEBUG_SEVERITY_WARNING_BIT, TL_DEBUG_SOURCE_THALLIUM_BIT, __VA_ARGS__)
#define TL_Warn_Vk(debugger, ...)   \
    TL_ReportMessage(debugger, TL_DEBUG_SEVERITY_WARNING_BIT, TL_DEBUG_SOURCE_VULKAN_BIT, __VA_ARGS__)

#define TL_Error(debugger, ...)     \
    TL_ReportMessage(debugger, TL_DEBUG_SEVERITY_ERROR_BIT, TL_DEBUG_SOURCE_THALLIUM_BIT, __VA_ARGS__)
#define TL_Error_Vk(debugger, ...)  \
    TL_ReportMessage(debugger, TL_DEBUG_SEVERITY_ERROR_BIT, TL_DEBUG_SOURCE_VULKAN_BIT, __VA_ARGS__)

#define TL_Fatal(debugger, ...)     \
    TL_ReportMessage(debugger, TL_DEBUG_SEVERITY_FATAL_BIT, TL_DEBUG_SOURCE_THALLIUM_BIT, __VA_ARGS__)

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif
