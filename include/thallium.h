/*
 *   Copyright (c) 2023 Jack Bennett
 *   All rights reserved.
 *
 *   Please see the LICENCE file for more information.
 */

//
//  ___________________________
// < THALLIUM GRAPHICS LIBRARY >
//  ---------------------------
//         \   ^__^
//          \  (oo)\_______
//             (__)\       )\/
//                 ||----w |
//                 ||     ||
//
// Contribute at https://github.com/kosude/thallium uwu
//
//

/**
 * @file thallium.h
 * @brief Public Thallium API header
 *
 * Include this file to gain access to all public Thallium functions and types.
 *
 * Generally, you only need to include this header. However, if you want to access the more low-level
 * Thallium abstractions of a certain graphics API, you may include the appropriate header, such as
 * `thallium_vulkan.h` for Vulkan. Again; this isn't required, but it is possible in case the surface-level
 * API does not provide all the functionality you need.
 *
 * You can also directly include the graphics API's own headers, as their include directories are publicly
 * included in each API module that is compiled.
 *
 * Thallium functions are prefixed with `th_`.
 */

#pragma once
#ifndef _THALLIUM_PUB_H
#define _THALLIUM_PUB_H
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

#include "thallium/debug.h"
#include "thallium/extension.h"
#include "thallium/renderer.h"
#include "thallium/version.h"

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif // !_THALLIUM_PUB_H
