/*
 *   Copyright (c) 2023 Jack Bennett
 *   All rights reserved.
 *
 *   Please see the LICENCE file for more information.
 */

/**
 * @file debugger.h
 * @brief Library compounds for configuring Thallium debug output.
 *
 * @defgroup debugger Library debuggers
 * @brief Functions pertaining to Thallium debugging
 *
 * Library compounds for configuring Thallium debug output with debuggers.
 *
 * Note that these are independent of the graphics API - for instance, you would still need to handle a
 * Vulkan debug messenger pipeline yourself.
 */

#pragma once
#ifndef __thallium_debugger_h__
#define __thallium_debugger_h__
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

#include "thallium/enums.h"


// ===========================================================================
//                           THALLIUM PUBLIC API
// ===========================================================================

/**
 * @ingroup debugger
 * @brief A structure used to debug renderers.
 *
 * This structure can be created and passed to renderer creation to enable runtime
 * debug messages - for instance, it is needed to recieve errors and warnings.
 *
 * Note that this is independent of API-specific debugging methods (such as OpenGL debug output).
 *
 * @warning Debuggers will not be used if the Thallium debug layer was not enabled on compilation.
 *
 * @sa @ref th_CreateDebugger()
 */
typedef struct th_Debugger_t th_Debugger_t;

/**
 * @ingroup debugger
 * @brief Create a debugger struct with the specified configuration options.
 *
 * This function creates a debugger structure with the specified configuration options.
 *
 * @param severities bit field of @ref th_DebugSeverity_t enumerator(s) - messages of these severities will be reported
 * @param types bit field of @ref th_DebugType_t enumerator(s) - messages of these types will be reported.
 * @return Pointer to the new debugger
 *
 * @sa @ref th_Debugger_t
 */
th_Debugger_t *th_CreateDebugger(
    const th_DebugSeverity_t severities,
    const th_DebugType_t types
);

/**
 * @ingroup debugger
 * @brief Free the given debugger object.
 *
 * This function frees the specified debugger object. You should set the pointer to NULL
 * after calling this function, as you would normally when calling free().
 *
 * @param debugger The debugger object to free.
 * @return @returnstatus
 *
 * @alwaysok
 */
int th_DestroyDebugger(
    th_Debugger_t *debugger
);

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif // !__thallium_debugger_h__
