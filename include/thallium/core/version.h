/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#pragma once
#ifndef __TL__core__version_h__
#define __TL__core__version_h__
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

#include "thallium/platform.h"

/**
 * @brief A structure to describe a version.
 *
 * This structure describes a version with major, minor, and patch values.
 */
typedef struct TL_Version_t {
    /// @brief Major version number
    uint16_t major;
    /// @brief Minor version number
    uint16_t minor;
    /// @brief Patch version number
    uint16_t patch;
} TL_Version_t;

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif
