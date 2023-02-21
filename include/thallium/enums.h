/*
 *   Copyright (c) 2023 Jack Bennett
 *   All rights reserved.
 *
 *   Please see the LICENCE file for more information.
 */

/**
 * @file enums.h
 * @brief Thallium enumerators
 *
 * Include this file to recieve definitions of all public Thallium API enumerators.
 */

#pragma once
#ifndef __thallium_enums_h__
#define __thallium_enums_h__
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus


// ===========================================================================
//                           THALLIUM PUBLIC API
// ===========================================================================

/**
 * @ingroup debugger
 * @brief Enumeration containing severities to describe debug messages.
 *
 * This enumeration contains severities to describe debug messages.
 *
 * @sa @ref th_Debugger_t
 * @sa @ref th_CreateDebugger()
 */
typedef enum th_DebugSeverity_t {
    /// @brief Includes log messages.
    THALLIUM_DEBUG_SEVERITY_VERBOSE_BIT =   0x10,
    /// @brief Includes notification output.
    THALLIUM_DEBUG_SEVERITY_NOTIF_BIT =     0x08,
    /// @brief Includes warning output.
    THALLIUM_DEBUG_SEVERITY_WARNING_BIT =   0x04,
    /// @brief Includes non-fatal error output.
    THALLIUM_DEBUG_SEVERITY_ERROR_BIT =     0x02,
    /// @brief Includes fatal error output.
    THALLIUM_DEBUG_SEVERITY_FATAL_BIT =     0x01,
    /// @brief Includes all debug messages severities.
    THALLIUM_DEBUG_SEVERITY_ALL_BIT =       0x1F
} th_DebugSeverity_t;

/**
 * @ingroup debugger
 * @brief Enumeration containing types/sources to describe debug messages.
 *
 * This enumeration contains message types/sources to describe debug messages.
 *
 * @sa @ref th_Debugger_t
 * @sa @ref th_CreateDebugger()
 */
typedef enum th_DebugType_t {
    /// @brief Includes core library messages (prefixed by `(th)`)
    THALLIUM_DEBUG_TYPE_CORE_BIT =      0x02,
    /// @brief Includes Vulkan debug messages, sent by Vulkan renderers (prefixed by `(thvk)`)
    THALLIUM_DEBUG_TYPE_VULKAN_BIT =    0x01,
    /// @brief Includes all debug message types.
    THALLIUM_DEBUG_TYPE_ALL_BIT =       0x1F
} th_DebugType_t;

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif // !__thallium_enums_h__
