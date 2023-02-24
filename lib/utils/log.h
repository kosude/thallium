/*
 *   Copyright (c) 2023 Jack Bennett
 *   All rights reserved.
 *
 *   Please see the LICENCE file for more information.
 */

#pragma once
#ifndef __thallium_log_h__
#define __thallium_log_h__
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

#include "thallium/fwd.h"

// Not documenting this as there are loads of functions which do very similar things and it makes
// the file wayyy too big.
/** @cond */

int th_Log(const th_Debugger_t *debugger, const char *format, ...);
int th_Log_Vk(const th_Debugger_t *debugger, const char *format, ...);

int th_Note(const th_Debugger_t *debugger, const char *format, ...);

int th_Hint(const th_Debugger_t *debugger, const char *format, ...);

int th_Warn(const th_Debugger_t *debugger, const char *format, ...);
int th_Warn_Vk(const th_Debugger_t *debugger, const char *format, ...);

int th_Error(const th_Debugger_t *debugger, const char *format, ...);
int th_Error_Vk(const th_Debugger_t *debugger, const char *format, ...);

int th_Fatal(const th_Debugger_t *debugger, const char *format, ...);

/** @endcond */

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif // !__thallium_log_h__
