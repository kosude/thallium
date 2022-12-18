/*
 *   Copyright (c) 2022 Jack Bennett
 *   All rights reserved.
 *
 *   Please see the LICENCE file for more information.
 */

/**
 * @file log.h
 * @brief Low-level functions for printing formatted log messages
 *
 * @copyright Copyright (c) 2022
 */

#pragma once
#ifndef _THALLIUM_LOG_H
#define _THALLIUM_LOG_H
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

/**
 * @brief Write a log message to stdout.
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
 * @param format String format
 * @param ... Variadic formatted string arguments
 */
void th_Fatal(
    const char *format,
    ...
);

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif // !_THALLIUM_LOG_H
