/*
 *   Copyright (c) 2023 Jack Bennett
 *   All rights reserved.
 *
 *   Please see the LICENCE file for more information.
 */

#include "proc.h"

#include <stdio.h>
#include <stdlib.h>

int th_KillProc(void) {
    fprintf(stderr, "HALT_AND_CATCH_FIRE ~libthallium\n");
    exit(EXIT_FAILURE);

    return 1;
}
