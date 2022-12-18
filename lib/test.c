/*
 *   Copyright (c) 2022 Jack Bennett
 *   All rights reserved.
 *
 *   Please see the LICENCE file for more information.
 */

#include "utils/log.h"

#include <stdio.h>

void th_TestFunc(int x) {
    printf("x = %d\n", x);

    printf("1 + 2 = %d\n", th_Test());
}
