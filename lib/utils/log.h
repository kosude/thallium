/*
 *   Copyright (c) 2022 Jack Bennett
 *   All rights reserved.
 *
 *   Please see the LICENCE file for more information.
 */

/**
 * @file log.h
 * @brief Low-level functions for printing formatted log messages.
 *
 * @copyright Copyright (c) 2022
 *
 * @defgroup utils_log Message logging
 * @brief Writing formatted messages to an output stream
 * @ingroup utils
 *
 * Low-level functions for printing formatted log messages.
 */

#pragma once
#ifndef _THALLIUM_LOG_H
#define _THALLIUM_LOG_H
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

/// @ingroup utils_log
/// @{

/**
 * @brief Write a log message to stdout.
 *
 * This function prints a log message to *stdout*, in the following format:
 * ```
 * (thallium!) <MSG>
 * ```
 *
 * The message is printed in the default text colour.
 *
 * @param format String format
 * @param ... Variadic formatted string arguments
 */
void th_Log(
    const char *format,
    ...
);

/**
 * @brief Write a notification message to stdout.
 *
 * This function prints a notification message to *stdout*, in the following format:
 * ```
 * (thallium!) <MSG>
 * ```
 *
 * The message is printed in a blue foreground colour.
 *
 * @param format String format
 * @param ... Variadic formatted string arguments
 */
void th_Note(
    const char *format,
    ...
);

/**
 * @brief Write a warning message to stdout.
 *
 * This function prints a warning message to *stdout*, in the following format:
 * ```
 * (thallium!) WARN: <MSG>
 * ```
 *
 * The message is printed in a yellow foreground colour.
 *
 * @param format String format
 * @param ... Variadic formatted string arguments
 */
void th_Warn(
    const char *format,
    ...
);

/**
 * @brief Write an error message to stderr.
 *
 * This function prints an error message to *stderr*, in the following format:
 * ```
 * (thallium!) ERROR: <MSG>
 * ```
 *
 * The message is printed in a red foreground colour.
 *
 * @param format String format
 * @param ... Variadic formatted string arguments
 */
void th_Error(
    const char *format,
    ...
);

/**
 * @brief Write a fatal error message to stderr.
 *
 * This function prints a fatal error message to *stderr*, in the following format:
 * ```
 * (thallium!) FATAL: <MSG>
 * ```
 *
 * Due to its implied importance, the message is printed in a red foreground colour on a yellow background.
 *
 * @param format String format
 * @param ... Variadic formatted string arguments
 */
void th_Fatal(
    const char *format,
    ...
);

/// @}

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif // !_THALLIUM_LOG_H
