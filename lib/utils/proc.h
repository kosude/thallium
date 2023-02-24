/*
 *   Copyright (c) 2023 Jack Bennett
 *   All rights reserved.
 *
 *   Please see the LICENCE file for more information.
 */

/**
 * @file proc.h
 * @brief Functions for managing the currently executing process.
 *
 * @defgroup utils_proc Process management
 * @brief Managing process
 * @ingroup utils
 *
 * Functions for managing the currently executing process.
 */

#pragma once
#ifndef __thallium_proc_h__
#define __thallium_proc_h__
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

/**
 * @ingroup utils_proc
 * @brief Kill the currently executing process.
 *
 * This function kills the currently executing process. This may be useful for when a fatal
 * error is thrown.
 *
 * @return @returnstatus
 *
 * @alwaysok
 */
int th_KillProc(void);

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif // !__thallium_proc_h__
