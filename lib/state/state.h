/*
 *   Copyright (c) 2023 Jack Bennett
 *   All rights reserved.
 *
 *   Please see the LICENCE file for more information.
 */

/**
 * @file state.h
 * @brief Global state variable declarations.
 */

#pragma once
#ifndef _THALLIUM_STATE_H
#define _THALLIUM_STATE_H
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

/**
 * @ingroup state
 * @brief Internal library state structure.
 */
typedef struct th_State_t {
    /// @brief th_DebugSeverity_t bit-field holding severities of messages to display.
    int debugSeverityFilter;
} th_State_t;

/**
 * @ingroup state
 * @brief Globally accessible instance of @ref th_State_t.
 */
extern th_State_t thallium;

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif // !_THALLIUM_STATE_H
