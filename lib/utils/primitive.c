/*
 *   Copyright (c) 2023 Jack Bennett
 *   All rights reserved.
 *
 *   Please see the LICENCE file for more information.
 */

#include "primitive.h"

#include <string.h>

int th_StringValueInArray(const char *const val, const char *const *arr, const unsigned int arr_size) {
    for (unsigned int i = 0; i < arr_size; i++) {
        if (!strcmp(arr[i], val)) {
            return 1;
        }
    }
    return 0;
}
