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

#include "thallium/fwd.h"

void TL_Log(const TL_Debugger_t *const debugger, const char *const format, ...);
void TL_Log_Vk(const TL_Debugger_t *const debugger, const char *const format, ...);

void TL_Note(const TL_Debugger_t *const debugger, const char *const format, ...);

void TL_Hint(const TL_Debugger_t *const debugger, const char *const format, ...);

void TL_Warn(const TL_Debugger_t *const debugger, const char *const format, ...);
void TL_Warn_Vk(const TL_Debugger_t *const debugger, const char *const format, ...);

void TL_Error(const TL_Debugger_t *const debugger, const char *const format, ...);
void TL_Error_Vk(const TL_Debugger_t *const debugger, const char *const format, ...);

void TL_Fatal(const TL_Debugger_t *const debugger, const char *const format, ...);

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif
