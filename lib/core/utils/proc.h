/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#pragma once
#ifndef __TL__utils__proc_h__
#define __TL__utils__proc_h__
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

/**
 * @brief Kill the currently executing process.
 *
 * This function kills the currently executing process. This may be useful for when a fatal
 * error is thrown.
 */
void TL_KillProc(void);

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif
