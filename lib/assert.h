/*
 *   Copyright (c) 2023 Jack Bennett
 *   All rights reserved.
 *
 *   Please see the LICENCE file for more information.
 */

/**
 * @file assert.h
 * @brief Internal assertion macros
 *
 * @defgroup assert Internal-use assertion macros
 * @brief Macros to assert values of expressions
 *
 * Internal assertion macros/functions
 */

#pragma once
#ifndef __thallium_assert_h__
#define __thallium_assert_h__
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

/** @cond */

// #ifdef THALLIUM_BUILD_Debug
#   define THALLIUM_ASSERTIONS_ENABLED
// #endif

#ifdef THALLIUM_ASSERTIONS_ENABLED
#   include <stdio.h>

    // __trap() is a portable debug break function.
#   if defined(_MSC_VER) /* MSVC */
#       define __trap() __debugbreak()
#   elif defined(__GNUC__) /* GCC/Clang/etc */
#       define __trap() __builtin_trap()
#   else /* some other compiler */
#       include <signal.h>
#       if defined(SIGTRAP) /* POSIX compliant */
#           define __trap() raise(SIGTRAP)
#       else /* non-POSIX compliant */
#           define __trap() raise(SIGABRT)
#       endif
#   endif

#   define TH_ASSERT(expr) \
    { \
        if (!(expr)) { \
            fprintf(stderr, \
                "Thallium assertion error: %s (%s:%d)\n", \
                #expr, __FILE__, __LINE__ \
            ); \
            __trap(); \
        } \
    }

#   ifdef THALLIUM_VULKAN_INCL
#       define TH_ASSERT_VK(expr) \
        { \
           TH_ASSERT(expr == VK_SUCCESS); \
        }
#   endif
#else
#   define TH_ASSERT(expr) expr
#   ifdef THALLIUM_VULKAN_INCL
#       define TH_ASSERT_VK(expr) expr
#   endif
#endif

/** @endcond */

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif //!__thallium_assert_h__
