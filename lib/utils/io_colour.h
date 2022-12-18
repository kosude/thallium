/*
 *   Copyright (c) 2022 Jack Bennett
 *   All rights reserved.
 *
 *   Please see the LICENCE file for more information.
 */

#pragma once
#ifndef _THALLIUM_IO_COLOUR_H
#define _THALLIUM_IO_COLOUR_H
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

// forward declaration of FILE
typedef struct _IO_FILE FILE;

/**
 * @brief Enumeration containing colour options for input/output
 */
typedef enum th_IOColour_t {
    /// @brief Black IO colour
    THALLIUM_IO_COLOUR_BLACK =              0x00,
    /// @brief Dark blue IO colour
    THALLIUM_IO_COLOUR_DARK_BLUE =          0x01,
    /// @brief Dark green IO colour
    THALLIUM_IO_COLOUR_DARK_GREEN =         0x02,
    /// @brief Light blue IO colour
    THALLIUM_IO_COLOUR_LIGHT_BLUE =         0x03,
    /// @brief Dark red IO colour
    THALLIUM_IO_COLOUR_DARK_RED =           0x04,
    /// @brief Magenta IO colour
    THALLIUM_IO_COLOUR_MAGENTA =            0x05,
    /// @brief Orange IO colour
    THALLIUM_IO_COLOUR_ORANGE =             0x06,
    /// @brief Light grey IO colour
    THALLIUM_IO_COLOUR_LIGHT_GREY =         0x07,
    /// @brief Grey IO colour
    THALLIUM_IO_COLOUR_GREY =               0x08,
    /// @brief Blue IO colour
    THALLIUM_IO_COLOUR_BLUE =               0x09,
    /// @brief Green IO colour
    THALLIUM_IO_COLOUR_GREEN =              0x0A,
    /// @brief Cyan IO colour
    THALLIUM_IO_COLOUR_CYAN =               0x0B,
    /// @brief Red IO colour
    THALLIUM_IO_COLOUR_RED =                0x0C,
    /// @brief Pink IO colour
    THALLIUM_IO_COLOUR_PINK =               0x0D,
    /// @brief Yellow IO colour
    THALLIUM_IO_COLOUR_YELLOW =             0x0E,
    /// @brief White IO colour
    THALLIUM_IO_COLOUR_WHITE =              0x0F
} th_IOColour_t;

/**
 * @brief Set input/output colours
 *
 * Pass 0xFF to either colour in order to skip it
 *
 * @param fg Foreground colour
 * @param bg Background colour
 * @param stream The stream to set the colour in (relevant for Unix)
 */
const void th_IOColourSet(
    const th_IOColour_t fg,
    const th_IOColour_t bg,
    FILE *stream
);

/**
 * @brief Reset input/output colours to default
 *
 * @param stream The stream to reset the colour in (relevant for Unix)
 */
const void th_IOColourDefaults(
    FILE *stream
);

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif // !_THALLIUM_IO_COLOUR_H
