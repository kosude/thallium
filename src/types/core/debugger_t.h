/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#pragma once
#ifndef __TL__internal__core__debugger_h__
#define __TL__internal__core__debugger_h__
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

#include "thallium/core/debugger.h"

typedef struct TL_Debugger_t {
    /// @brief Bit field of severity flags - messages of these severities will be reported
    TL_DebugSeverityFlags_t severities;
    /// @brief Bit field of source flags - messages from these sources will be reported
    TL_DebugSourceFlags_t sources;

    /// @brief User-set callback function - can be NULL
    TL_DebugCallbackfn_t user_callback;
    /// @brief User-set pointer to send to the callback - can be NULL
    void *user_pointer;
} TL_Debugger_t;

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif
