/*
 *   Copyright (c) 2023 Jack Bennett.
 *   All Rights Reserved.
 *
 *   See the LICENCE file for more information.
 */

#include "proc.h"

#include <stdio.h>
#include <stdlib.h>

void TL_KillProc(void) {
    fprintf(stderr, "HALT_AND_CATCH_FIRE ~libthallium\n");
    exit(EXIT_FAILURE);
}
