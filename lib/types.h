/*
 *   Copyright (c) 2023 Jack Bennett
 *   All rights reserved.
 *
 *   Please see the LICENCE file for more information.
 */

/**
 * @file types.h
 * @brief Opaque type definitions
 *
 * @defgroup opaque_types Opaque type definitions
 * @brief Definitions for public opaque structures
 *
 * Internal definitions for public opaque types and structures.
 */

#pragma once
#ifndef _THALLIUM_RENDERER_H
#define _THALLIUM_RENDERER_H
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

/**
 * @ingroup opaque_types
 * @brief A structure to represent a graphics API.
 *
 * This opaque structure provides a representation of a certain graphics API.
 */
typedef struct th_Renderer_t {
    /// @brief The ID of the graphics API specified to the renderer.
    int apiId;

    /// @brief Render system - the API-specific data held within the renderer.
    /// This structure is defined in the relevant module.
    void *renderSystem;
} th_Renderer_t;

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif // !_THALLIUM_TYPES_H
