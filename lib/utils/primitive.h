/*
 *   Copyright (c) 2023 Jack Bennett
 *   All rights reserved.
 *
 *   Please see the LICENCE file for more information.
 */

/**
 * @file primitive.h
 * @brief General-use helper functions for handling primitive types.
 *
 * @defgroup utils_primitive Primitive helper functions
 * @brief General-use internal helper functions for primitive types
 * @ingroup utils
 *
 * General-use helper functions for handling primitive types.
 */

#pragma once
#ifndef __thallium_primitive_h__
#define __thallium_primitive_h__
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

/**
 * @ingroup utils_primitive
 * @brief Check if a string value is in the given array.
 *
 * This function looks for the specified value in the given array.
 *
 * @param val Value to look for
 * @param arr Array to consider
 * @param arr_size Size of array `arr`
 * @return 0 if the value was not found in the array
 * @return 1 if the value was found in the array
 */
const int th_StringValueInArray(
    const char *const val,
    const char *const *arr,
    const unsigned int arr_size
);

#ifdef __cplusplus
    }
#endif // __cplusplus
#endif // !__thallium_primitive_h__
