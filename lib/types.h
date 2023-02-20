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
#ifndef __thallium_types_h__
#define __thallium_types_h__
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

    /// @brief Debugger to read configuration from when reporting debug messages.
    /// (Can be NULL - this just means no debug messages)
    th_Debugger_t *debugger;
} th_Renderer_t;

/**
 * @ingroup opaque_types
 * @brief A structure used to debug renderers.
 *
 * This opaque structure provides debugging configuration options for Thallium renderers.
 */
typedef struct th_Debugger_t {
    /// @brief th_DebugSeverity_t bit-field holding severities of messages to display.
    int debugSeverityFilter;
} th_Debugger_t;

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif // !__thallium_types_h__
