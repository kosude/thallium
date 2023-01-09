/*
 *   Copyright (c) 2023 Jack Bennett
 *   All rights reserved.
 *
 *   Please see the LICENCE file for more information.
 */

#include "primitive.h"

#include <string.h>

const int th_StringValueInArray(const char *const val, const char *const *arr, const unsigned int arrSize) {
    for (unsigned int i = 0; i < arrSize; i++) {
        if (!strcmp(arr[i], val)) {
            return 1;
        }
    }
    return 0;
}
