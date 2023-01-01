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
 * @copyright Copyright (c) 2023
 *
 * @defgroup utils_proc Process management
 * @brief Managing process
 * @ingroup utils
 *
 * Functions for managing the currently executing process.
 */

#pragma once
#ifndef _THALLIUM_PROC_H
#define _THALLIUM_PROC_H
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

/// @ingroup utils_proc
/// @{

/**
 * @brief Kill the currently executing process.
 *
 * This function kills the currently executing process. This may be useful for when a fatal
 * error is thrown.
 */
const void th_KillProc();

/// @}

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif // !_THALLIUM_PROC_H
