/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#pragma once
#ifndef __TL__core__types_h__
#define __TL__core__types_h__
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

#include "thallium/enums.h"
#include "thallium/platform.h"

typedef struct TL_Debugger_t {
    /// @brief Bit field of severity flags - messages of these severities will be reported
    TL_DebugSeverityFlags_t severities;
    /// @brief Bit field of source flags - messages from these sources will be reported
    TL_DebugSourceFlags_t sources;
} TL_Debugger_t;

typedef struct TL_Context_t {
    /// @brief Memory offset from the .data pointer to the Vulkan-specific block of data
    size_t vulkan_offset;

    /// @brief The size of the memory allocated at the .data pointer
    size_t data_size;
    /// @brief Context API-aware data
    void *data;
} TL_Context_t;

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif
