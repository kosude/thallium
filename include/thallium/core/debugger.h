/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#pragma once
#ifndef __TL__core__debugger_h__
#define __TL__core__debugger_h__
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

#include "thallium/enums.h"
#include "thallium/fwd.h"

/**
 * @brief A structure used to debug Thallium operations.
 *
 * This structure can be created and passed to Thallium functions to receive debug messages. This is necessary to recieve errors and warnings, for
 * example.
 *
 * @sa @ref TL_CreateDebugger()
 * @sa @ref TL_DestroyDebugger()
 * @sa @ref TL_DebuggerDescriptor_t
 */
typedef struct TL_Debugger_t TL_Debugger_t;

/**
 * @brief A structure describing a Thallium debugger object to be created.
 *
 * This structure describes a [debugger object](@ref TL_Debugger_t) to be created.
 */
typedef struct TL_DebuggerDescriptor_t {
    /// @brief Bit field of severity flags - messages of these severities will be reported
    TL_DebugSeverityFlags_t severities;
    /// @brief Bit field of source flags - messages from these sources will be reported
    TL_DebugSourceFlags_t sources;

    // TODO: add a callback
} TL_DebuggerDescriptor_t;

/**
 * @brief A structure describing a Thallium debugger attachment.
 *
 * This structure describes a debugger attachment.
 *
 * A debug attachment refers to the idea of immutably 'attaching' a debugger to a context at creation time, allowing for more detailed debug output.
 * More specifically, this enables API-specific debug functionality. For example, Vulkan
 * [debug utils](https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VK_EXT_debug_utils.html) are enabled for use by Vulkan renderers.
 */
typedef struct TL_DebuggerAttachmentDescriptor_t {
    /// @brief Debugger to link the attachment to
    const TL_Debugger_t *debugger;
} TL_DebuggerAttachmentDescriptor_t;

/**
 * @brief Create a debugger with the specified configuration options.
 *
 * This function creates a debugger with the specified configuration options.
 *
 * @param descriptor a @ref TL_DebuggerDescriptor_t descriptor structure.
 *
 * @return Pointer to the new debugger
 *
 * @sa @ref th_Debugger_t
 */
TL_Debugger_t *TL_CreateDebugger(
    const TL_DebuggerDescriptor_t descriptor
);

/**
 * @brief Free the given debugger object.
 *
 * This function frees the specified debugger object.
 *
 * @param debugger The debugger object to free.
 */
void TL_DestroyDebugger(
    TL_Debugger_t *const debugger
);

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif
