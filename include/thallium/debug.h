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
 * @brief Enum for explicit function return statuses.
 *
 * This enumeration provides explicit function return statuses.
 * The only two available values are '1' for OK, and '0' for an error.
 *
 * Using this, you may choose to assert `!th_ExampleFunction()` to check if function `th_ExampleFunction()`
 * returned successfully.
 */
typedef enum th_Status_t {
    /// @brief The function returned successfully
    THALLIUM_STATUS_OK =    0x01,
    /// @brief The function encountered a severe or fatal error
    THALLIUM_STATUS_ERROR = 0x00
} th_Status_t;

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
    THALLIUM_DEBUG_SEVERITY_VERBOSE_BIT =   0x10,
    /// @brief Includes notification output.
    THALLIUM_DEBUG_SEVERITY_NOTIF_BIT =     0x08,
    /// @brief Includes warning output.
    THALLIUM_DEBUG_SEVERITY_WARNING_BIT =   0x04,
    /// @brief Includes non-fatal error output.
    THALLIUM_DEBUG_SEVERITY_ERROR_BIT =     0x02,
    /// @brief Includes fatal error output.
    THALLIUM_DEBUG_SEVERITY_FATAL_BIT =     0x01,
    /// @brief Includes all debug messages.
    THALLIUM_DEBUG_SEVERITY_ALL_BIT =       0xFF
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
const th_Status_t th_ConfigureDebugMessageFilter(
    const th_DebugSeverity_t severityBits
);

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif // !_THALLIUM_PUB_DEBUG_H
