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
    const int major;
    /// @brief Minor version number
    const int minor;
    /// @brief Patch/build number
    const int patch;
} th_Version_t;

/**
 * @ingroup utils
 * @brief Convert the given version to a string and return it.
 *
 * Convert the given version to a string and return it into `out_string`.
 *
 * @param version The version to stringify
 * @param out_string Pointer to the string to update
 */
void th_VersionToString(
    const th_Version_t version,
    char *out_string
);

/**
 * @ingroup utils
 * @brief Retrieve, as a human-readable string, the version of Thallium that was compiled.
 *
 * This function returns the version of Thallium that was compiled in the format of a human-readable string.
 *
 * @return Stringified Thallium version
 */
const char *th_GetThalliumVersionString(void);

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif // !__thallium_version_h__
