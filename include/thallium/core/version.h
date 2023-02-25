/*
 *   Copyright (c) 2023 Jack Bennett
 *   All rights reserved.
 *
 *   Please see the LICENCE file for more information.
 */

/**
 * @file version.h
 * @brief Version structure definition and relevant function(s)
 */

#pragma once
#ifndef __thallium_version_h__
#define __thallium_version_h__
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus


// ===========================================================================
//                           THALLIUM PUBLIC API
// ===========================================================================

/**
 * @ingroup utils
 * @brief A structure to describe a version.
 *
 * This structure describes a version with major, minor, and patch values.
 */
typedef struct th_Version_t {
    /// @brief Major version number
    int major;
    /// @brief Minor version number
    int minor;
    /// @brief Patch/build number
    int patch;
} th_Version_t;

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif // !__thallium_version_h__
