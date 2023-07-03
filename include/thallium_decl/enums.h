/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#pragma once
#ifndef __TL__thallium__enums_h__
#define __TL__thallium__enums_h__
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

/**
 * @brief Enumeration containing graphics APIs for renderers.
 *
 * This enumeration contains all graphics APIs for renderers. Not all enumerations will necessarily be accepted by renderers on creation; ensure
 * that the appropriate module or modules were added to the build process when compiling the Thallium library.
 */
typedef enum TL_RendererAPIFlags_t {
    /// @brief [Vulkan GPU API](https://www.vulkan.org/)
    TL_RENDERER_API_VULKAN_BIT =    0x10,

    /// @brief Null API. Not to be used in valid Thallium applications.
    TL_RENDERER_API_NULL_BIT =      0x00,
} TL_RendererAPIFlags_t;

/**
 * @brief Enumeration containing severities to describe debug messages.
 *
 * This enumeration contains severities to describe debug messages.
 *
 * @sa @ref TL_Debugger_t
 * @sa @ref TL_DebuggerDescriptor_t
 * @sa @ref TL_DebuggerCreate()
 */
typedef enum TL_DebugSeverityFlags_t {
    /// @brief Log messages
    TL_DEBUG_SEVERITY_VERBOSE_BIT = 0x10,
    /// @brief Notification messages
    TL_DEBUG_SEVERITY_NOTIF_BIT =   0x08,
    /// @brief Warning messages
    TL_DEBUG_SEVERITY_WARNING_BIT = 0x04,
    /// @brief Error messages
    TL_DEBUG_SEVERITY_ERROR_BIT =   0x02,
    /// @brief Fatal error messages
    TL_DEBUG_SEVERITY_FATAL_BIT =   0x01,

    /// @brief All message severities
    TL_DEBUG_SEVERITY_ALL_BIT =     0x1f,
} TL_DebugSeverityFlags_t;

/**
 * @brief Enumeration containing sources to describe debug messages.
 *
 * This enumeration contains message sources to describe debug messages.
 *
 * @sa @ref TL_Debugger_t
 * @sa @ref TL_DebuggerCreate()
 */
typedef enum TL_DebugSourceFlags_t {
    /// @brief Messages reported by core Thallium functions
    TL_DEBUG_SOURCE_THALLIUM_BIT =  0x20,
    /// @brief Messages reported by Vulkan renderer systems
    TL_DEBUG_SOURCE_VULKAN_BIT =    0x10,

    /// @brief All message sources
    TL_DEBUG_SOURCE_ALL_BIT =       0x3f,
} TL_DebugSourceFlags_t;

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif
