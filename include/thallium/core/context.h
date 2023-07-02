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
 * @sa @ref TL_ContextCreate()
 * @sa @ref TL_ContextDestroy()
 * @sa @ref TL_ContextDescriptor_t
 */
typedef struct TL_Context_t TL_Context_t;

/**
 * @brief A structure describing a Thallium context object to be created with @ref TL_ContextCreate().
 *
 * This structure describes a [context object](@ref TL_Context_t) to be created.
 *
 * @sa @ref TL_Context_t
 * @sa @ref TL_ContextCreate()
 * @sa @ref TL_ContextDestroy()
 */
typedef struct TL_ContextDescriptor_t {
    /// @brief NULL or a descriptor describing a debug attachment for the context.
    const TL_DebuggerAttachmentDescriptor_t *debug_attachment_descriptor;
} TL_ContextDescriptor_t;

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
 * @sa @ref TL_ContextDestroy()
 */
TL_Context_t *TL_ContextCreate(
    const TL_ContextDescriptor_t context_descriptor,
    const TL_Debugger_t *const debugger
);

/**
 * @brief Free space allocated for the given Thallium context object.
 *
 * This function frees the space allocated for the given Thallium [context object](@ref TL_Context_t) with @ref TL_ContextCreate,
 *
 * @param context Context object to destroy
 *
 * @sa @ref TL_Context_t
 * @sa @ref TL_ContextCreate()
 */
void TL_ContextDestroy(
    TL_Context_t *const context
);

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif
