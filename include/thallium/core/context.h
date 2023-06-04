/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#pragma once
#ifndef __TL__core__context_h__
#define __TL__core__context_h__
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

#include "thallium/fwd.h"

/**
 * @brief An opaque object to hold single context state for the Thallium library instance.
 *
 * This opaque struct holds context state for the application. There can only be one context per program.
 *
 * 'Context state' refers to any single API-specific objects, such as Vulkan instances. As having multiple of these objects in an application is
 * messy, it is avoided by restricting the creation of a Thallium context to only one.
 *
 * @sa @ref TL_CreateContext()
 * @sa @ref TL_DestroyContext()
 */
typedef struct TL_Context_t TL_Context_t;

/**
 * @brief Allocate space for and create a Thallium context object.
 *
 * This function creates a heap-allocated opaque Thallium context object and returns its handle.
 *
 * @param debugger NULL or a debugger for function debugging
 *
 * @return Handle to the new Thallium context
 *
 * @sa @ref TL_Context_t
 * @sa @ref TL_DestroyContext()
 */
TL_Context_t *TL_CreateContext(
    const TL_Debugger_t *const debugger
);

/**
 * @brief Free space allocated for the given Thallium context object.
 *
 * This function frees the space allocated for the given Thallium [context object](@ref TL_Context_t) with @ref TL_CreateContext,
 *
 * @param context Context object to destroy
 *
 * @sa @ref TL_Context_t
 * @sa @ref TL_CreateContext()
 */
void TL_DestroyContext(
    TL_Context_t *const context
);

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif
