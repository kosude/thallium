/*
 *   Copyright (c) 2023 Jack Bennett
 *   All rights reserved.
 *
 *   Please see the LICENCE file for more information.
 */

/*
//
//          88
//   ,d     88
//   88     88
// MM88MMM  88,dPPYba,
//   88     88P'    "8a
//   88     88       88
//   88,    88       88
//   "Y888  88       88  888888888
//
//  ______________________________________________________
// < Contribute at https://github.com/kosude/thallium uwu >
//  ------------------------------------------------------
//         \   ^__^
//          \  (oo)\_______
//             (__)\       )\/
//                 ||----w |
//                 ||     ||
//
*/

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
#ifndef __thallium_h__
#define __thallium_h__
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

#include "thallium/enums.h"

#include "thallium/core/debugger.h"
#include "thallium/core/renderer_config.h"
#include "thallium/core/renderer.h"
#include "thallium/core/version.h"

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif // !__thallium_h__
