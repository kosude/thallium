/*
 *   Copyright (c) 2023 Jack Bennett
 *   All rights reserved.
 *
 *   Please see the LICENCE file for more information.
 */

#include "proc.h"

#include "thallium/debug.h"

#include <stdio.h>
#include <stdlib.h>

const int th_KillProc() {
    fprintf(stderr, "(thallium!) HALT AND CATCH FIRE -- invoked by libthallium\n");
    exit(EXIT_FAILURE);

    return 1;
}
