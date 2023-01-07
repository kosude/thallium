/*
 *   Copyright (c) 2023 Jack Bennett
 *   All rights reserved.
 *
 *   Please see the LICENCE file for more information.
 */

/**
 * @file debug.h
 * @brief Library compounds for configuring Thallium debug output.
 *
 * @defgroup debug Library debug configuration
 * @brief Functions pertaining to Thallium debugging
 *
 * Library compounds for configuring Thallium debug output.
 *
 * Note that these are independent of the graphics API - for instance, you would still need to handle a
 * Vulkan debug messenger pipeline yourself.
 */

#pragma once
#ifndef _THALLIUM_PUB_DEBUG_H
#define _THALLIUM_PUB_DEBUG_H
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

/**
 * @ingroup debug
 * @brief Enumeration containing severities to describe debug messages.
 *
 * This enumeration contains severities to describe debug messages.
 *
 * @sa @ref th_ConfigureDebugMessageFilter()
 */
typedef enum th_DebugSeverity_t {
    /// @brief Includes log messages.
    /* 00010000 */ THALLIUM_DEBUG_SEVERITY_VERBOSE_BIT =   0x10,
    /// @brief Includes notification output.
    /* 00001000 */ THALLIUM_DEBUG_SEVERITY_NOTIF_BIT =     0x08,
    /// @brief Includes warning output.
    /* 00000100 */ THALLIUM_DEBUG_SEVERITY_WARNING_BIT =   0x04,
    /// @brief Includes non-fatal error output.
    /* 00000010 */ THALLIUM_DEBUG_SEVERITY_ERROR_BIT =     0x02,
    /// @brief Includes fatal error output.
    /* 00000001 */ THALLIUM_DEBUG_SEVERITY_FATAL_BIT =     0x01,
    /// @brief Includes all debug messages.
    /* 00011111 */ THALLIUM_DEBUG_SEVERITY_ALL_BIT =       0x1F
} th_DebugSeverity_t;

/**
 * @ingroup debug
 * @brief Select certain types of debugging messages to be displayed by Thallium.
 *
 * This function enables Thallium debug output with the specified criteria.
 *
 * A list of error severities can be seen in the documentation for the @ref th_DebugSeverity_t enumeration.
 *
 * @param severityBits bit field of @ref th_DebugSeverity_t enumerator(s)
 * @return @returnstatus
 *
 * @alwaysok
 *
 * @sa @ref th_DebugSeverity_t
 */
const int th_ConfigureDebugMessageFilter(
    const th_DebugSeverity_t severityBits
);

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif // !_THALLIUM_PUB_DEBUG_H
